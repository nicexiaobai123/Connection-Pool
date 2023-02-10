#pragma once
#include "public.h"
#include "Connection.h"
class ConnectionPool
{
public:
	// 单例 #2 提供一个获取唯一实例的方法
	static ConnectionPool* getConnectionPool();
	// 给外部提供接口，从连接池获取一个可用的空闲连接
	shared_ptr<Connection> getConnection();
private:
	// 单例 #1 构造函数私有化,删除拷贝构造和赋值
	ConnectionPool();
	ConnectionPool(const ConnectionPool&) = delete;
	ConnectionPool& operator=(const ConnectionPool&) = delete;
private:
	// 加载配置文件
	bool loadConfigFile();
	// 运行在独立线程中，用于生产新的连接
	void produceConnectionTask();
	// 定时扫描，扫描超过maxIdleTime时间的空闲连接，进行连接回收
	void scannerConnectionTask();
private:
	string _ip;
	unsigned short _port;
	string _user;
	string _password;
	string _dbname;
	int _initSize;			// 初始连接量
	int _maxSize;			// 最大连接量
	int _maxIdleTime;		// 最大空闲时间
	int _connectionTimeOut;	// 获取连接超时时间

	queue<Connection*> _connQue; // 存储连接的队列 -- 池子
	mutex _queueMutex;			 // 维护连接队列的线程安全互斥锁
	condition_variable cv;		 // 设置条件变量，用于生产者-消费者线程同步通信
	atomic_int _connCnt;		 // 记录连接池所创建的Connection连接的总数量
};