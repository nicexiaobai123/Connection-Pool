#pragma once
#include "public.h"
class Connection
{
public:
	// ��ʼ�����ݿ����� 
	Connection() { _conn = mysql_init(nullptr); }
	// �ͷ����ݿ�������Դ 
	~Connection() { if (_conn != nullptr) mysql_close(_conn); }
	// �������ݿ� 
	bool connect(string ip, unsigned short port, string user, string password, string dbname)
	{
		MYSQL* p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0); 
		return p != nullptr;
	}
	// ���²��� insert��delete��update 
	bool update(string sql) 
	{ 
		if (mysql_query(_conn, sql.c_str())) 
		{
			LOG("����ʧ��:" + sql);
			//cout << mysql_error(_conn) << endl;
			return false; 
		}
		return true; 
	}
	// ��ѯ���� select 
	MYSQL_RES* query(string sql) 
	{
		if (mysql_query(_conn, sql.c_str())) 
		{ 
			LOG("��ѯʧ��:" + sql); 
			return nullptr; 
		}
		return mysql_use_result(_conn);
	}
	// ����������ʼʱ��
	void refreshAliveTime() { _aliveStartTime = clock(); }
	// ��ȡ���ʱ��
	clock_t getAliveTime() { return clock() - _aliveStartTime; }
private:
	// ��ʾ��MySQL Server��һ������
	MYSQL* _conn;
	// ��¼�������״̬�����ʼ���ʱ��
	clock_t _aliveStartTime;
};