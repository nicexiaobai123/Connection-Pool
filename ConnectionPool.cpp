#include "ConnectionPool.h"
// ��������ȡΨһʵ��
ConnectionPool* ConnectionPool::getConnectionPool()
{
	// ���� #3 ���þ�̬����������
	static ConnectionPool singlePool;
	return &singlePool;
}

// ���������ļ� mysql.ini
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
		if (index == -1) continue;	// ��Ч���ã��޵��ں�

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

// ���캯��
ConnectionPool::ConnectionPool()
{
	// ���������ļ� -- ��Ա������ֵ
	if (!loadConfigFile()) return;

	// ��ʼ������
	for (int i = 0; i < _initSize; i++)
	{
		Connection* conn = new Connection();
		conn->connect(_ip, _port, _user, _password, _dbname);
		conn->refreshAliveTime();	// �������֮ǰ����������ʼʱ��
		_connQue.push(conn);
		_connCnt++;
	}

	// �����������̣߳�����"����"
	thread produce(bind(&ConnectionPool::produceConnectionTask, this));
	// ����ɨ���̣߳��رտ�������
	thread scanner(bind(&ConnectionPool::scannerConnectionTask, this));
	produce.detach();
	scanner.detach();
}

// �������̣߳����������µ�����
void ConnectionPool::produceConnectionTask()
{
	// һֱ׼������������
	while (true)
	{
		unique_lock<mutex> lock(_queueMutex);

		// ������������������
		while (!_connQue.empty()) cv.wait(lock);

		// û�����ˣ����������ӣ�ǰ���ǵ�ǰ���Ӳ��������������
		if(_connCnt < _maxSize)
		{
			Connection* conn = new Connection();
			conn->connect(_ip, _port, _user, _password, _dbname);
			conn->refreshAliveTime();	// �������֮ǰ����������ʼʱ��
			_connQue.push(conn);
			_connCnt++;
		}
		// ֪ͨ�������߳�
		cv.notify_all();
	}
}

// ���ⲿ�ṩ�ӿڣ������ӳػ�ȡһ�����õĿ������� --- ������
shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex> lock(_queueMutex);
	while (_connQue.empty())
	{
		// �ȴ����ӳ�ʱ
		if (cv_status::timeout == cv.wait_for(lock, ::chrono::milliseconds(_connectionTimeOut)))
		{
			if (_connQue.empty())
			{
				LOG("��ȡ�������ӳ�ʱ...");
				return nullptr;
			}
		}
	}
	// ��ȡһ������
	// ����ָ�����Զ���ɾ���������Զ�����ʱ�����ӻع鵽"��"�У�������ֱ������(close��)
	shared_ptr<Connection> sp(_connQue.front(),
		[this](Connection* conn)->void {
			// ���ڷ�����Ӧ���̵߳��ã��迼�ǵ��̰߳�ȫ
			unique_lock<mutex> lock(_queueMutex);	
			conn->refreshAliveTime();	// �������֮ǰ����������ʼʱ��
			_connQue.push(conn);
		});
	_connQue.pop();
	// ֪ͨ�����߳�����������
	cv.notify_all();	
	return sp;
}

// ��ʱɨ�裬ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ��������ӻ���
void ConnectionPool::scannerConnectionTask()
{
	while (true)
	{
		// ����һֱɨ�裬����ÿ��"һ��ʱ��"ɨ��
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		// ��һ�����У���ͷ��"����"����ʱ������õģ������ͷ��"����"û�г�ʱ���Ǻ�������Ӷ����ᳬʱ
		while (_connCnt > _initSize && !_connQue.empty())
		{
			unique_lock<mutex> lock(_queueMutex);
			Connection* p = _connQue.front();
			if (p->getAliveTime() >= _maxIdleTime * 1000)
			{
				_connCnt--;
				_connQue.pop();
				delete p;	// �ر�����
			}
			break;			// ��ͷ����û�г���
		}
	}
}
