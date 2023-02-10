#pragma once
#include "public.h"
#include "Connection.h"
class ConnectionPool
{
public:
	// ���� #2 �ṩһ����ȡΨһʵ���ķ���
	static ConnectionPool* getConnectionPool();
	// ���ⲿ�ṩ�ӿڣ������ӳػ�ȡһ�����õĿ�������
	shared_ptr<Connection> getConnection();
private:
	// ���� #1 ���캯��˽�л�,ɾ����������͸�ֵ
	ConnectionPool();
	ConnectionPool(const ConnectionPool&) = delete;
	ConnectionPool& operator=(const ConnectionPool&) = delete;
private:
	// ���������ļ�
	bool loadConfigFile();
	// �����ڶ����߳��У����������µ�����
	void produceConnectionTask();
	// ��ʱɨ�裬ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ��������ӻ���
	void scannerConnectionTask();
private:
	string _ip;
	unsigned short _port;
	string _user;
	string _password;
	string _dbname;
	int _initSize;			// ��ʼ������
	int _maxSize;			// ���������
	int _maxIdleTime;		// ������ʱ��
	int _connectionTimeOut;	// ��ȡ���ӳ�ʱʱ��

	queue<Connection*> _connQue; // �洢���ӵĶ��� -- ����
	mutex _queueMutex;			 // ά�����Ӷ��е��̰߳�ȫ������
	condition_variable cv;		 // ������������������������-�������߳�ͬ��ͨ��
	atomic_int _connCnt;		 // ��¼���ӳ���������Connection���ӵ�������
};