#pragma once
#include "public.h"
class Connection
{
public:
	// 初始化数据库连接 
	Connection() { _conn = mysql_init(nullptr); }
	// 释放数据库连接资源 
	~Connection() { if (_conn != nullptr) mysql_close(_conn); }
	// 连接数据库 
	bool connect(string ip, unsigned short port, string user, string password, string dbname)
	{
		MYSQL* p = mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0); 
		return p != nullptr;
	}
	// 更新操作 insert、delete、update 
	bool update(string sql) 
	{ 
		if (mysql_query(_conn, sql.c_str())) 
		{
			LOG("更新失败:" + sql);
			//cout << mysql_error(_conn) << endl;
			return false; 
		}
		return true; 
	}
	// 查询操作 select 
	MYSQL_RES* query(string sql) 
	{
		if (mysql_query(_conn, sql.c_str())) 
		{ 
			LOG("查询失败:" + sql); 
			return nullptr; 
		}
		return mysql_use_result(_conn);
	}
	// 更新连接起始时间
	void refreshAliveTime() { _aliveStartTime = clock(); }
	// 获取存活时间
	clock_t getAliveTime() { return clock() - _aliveStartTime; }
private:
	// 表示和MySQL Server的一条连接
	MYSQL* _conn;
	// 记录进入空闲状态后的起始存活时间
	clock_t _aliveStartTime;
};