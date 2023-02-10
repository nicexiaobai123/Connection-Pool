#include "ConnectionPool.h"
// 单例：获取唯一实例
ConnectionPool* ConnectionPool::getConnectionPool()
{
	// 单例 #3 利用静态变量的特性
	static ConnectionPool singlePool;
	return &singlePool;
}

// 加载配置文件 mysql.ini
bool ConnectionPool::loadConfigFile()
{
	//unique_ptr<FILE, function<void(FILE*)>> fp(fopen("mysql.ini", "r"), [](FILE* p)->void {fclose(p); });
	FILE* fp = fopen("mysql.ini", "r");
	if (fp == nullptr) {
		LOG("mysql.ini file is not exist");
		return false;
	}
	while (!feof(fp)) {
		char line[1024] = { 0 };
		fgets(line, 1024, fp);
		string str = line;
		int index = str.find('=', 0);
		if (index == -1) continue;	// 无效配置，无等于号

		int endIndex = str.find('\n', index);
		string key = str.substr(0, index);
		string value = str.substr(index + 1, endIndex - index - 1);
		if (key == "ip") _ip = value;
		else if (key == "port") _port = atoi(value.c_str());
		else if (key == "user") _user = value;
		else if (key == "password") _password = value;
		else if (key == "dbname") _dbname = value;
		else if (key == "initSize") _initSize = atoi(value.c_str());
		else if (key == "maxSize") _maxSize = atoi(value.c_str());
		else if (key == "maxIdleTime") _maxIdleTime = atoi(value.c_str());
		else if (key == "connectionTimeOut") _connectionTimeOut = atoi(value.c_str());
	}
	fclose(fp);
	return true;
}

// 构造函数
ConnectionPool::ConnectionPool()
{
	// 加载配置文件 -- 成员变量赋值
	if (!loadConfigFile()) return;

	// 初始连接量
	for (int i = 0; i < _initSize; i++)
	{
		Connection* conn = new Connection();
		conn->connect(_ip, _port, _user, _password, _dbname);
		conn->refreshAliveTime();	// 进入队列之前更新连接起始时间
		_connQue.push(conn);
		_connCnt++;
	}

	// 启动生产者线程，生产"连接"
	thread produce(bind(&ConnectionPool::produceConnectionTask, this));
	// 启动扫描线程，关闭空闲连接
	thread scanner(bind(&ConnectionPool::scannerConnectionTask, this));
	produce.detach();
	scanner.detach();
}

// 生产者线程，用于生产新的连接
void ConnectionPool::produceConnectionTask()
{
	// 一直准备生产新连接
	while (true)
	{
		unique_lock<mutex> lock(_queueMutex);

		// 队列有连接则不生产了
		while (!_connQue.empty()) cv.wait(lock);

		// 没连接了，生产新连接，前提是当前连接不超过最大连接数
		if(_connCnt < _maxSize)
		{
			Connection* conn = new Connection();
			conn->connect(_ip, _port, _user, _password, _dbname);
			conn->refreshAliveTime();	// 进入队列之前更新连接起始时间
			_connQue.push(conn);
			_connCnt++;
		}
		// 通知消费者线程
		cv.notify_all();
	}
}

// 给外部提供接口，从连接池获取一个可用的空闲连接 --- 消费者
shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex> lock(_queueMutex);
	while (_connQue.empty())
	{
		// 等待连接超时
		if (cv_status::timeout == cv.wait_for(lock, ::chrono::milliseconds(_connectionTimeOut)))
		{
			if (_connQue.empty())
			{
				LOG("获取空闲连接超时...");
				return nullptr;
			}
		}
	}
	// 获取一个连接
	// 智能指针需自定义删除器，在自动析构时让连接回归到"池"中，而不是直接析构(close掉)
	shared_ptr<Connection> sp(_connQue.front(),
		[this](Connection* conn)->void {
			// 是在服务器应用线程调用，需考虑到线程安全
			unique_lock<mutex> lock(_queueMutex);	
			conn->refreshAliveTime();	// 进入队列之前更新连接起始时间
			_connQue.push(conn);
		});
	_connQue.pop();
	// 通知其他线程生产或消费
	cv.notify_all();	
	return sp;
}

// 定时扫描，扫描超过maxIdleTime时间的空闲连接，进行连接回收
void ConnectionPool::scannerConnectionTask()
{
	while (true)
	{
		// 并非一直扫描，而是每隔"一段时间"扫描
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		// 是一个队列，队头的"连接"空闲时间是最久的，如果队头的"连接"没有超时，那后面的连接都不会超时
		while (_connCnt > _initSize && !_connQue.empty())
		{
			unique_lock<mutex> lock(_queueMutex);
			Connection* p = _connQue.front();
			if (p->getAliveTime() >= _maxIdleTime * 1000)
			{
				_connCnt--;
				_connQue.pop();
				delete p;	// 关闭连接
			}
			break;			// 队头连接没有超过
		}
	}
}
