#include "connection.h"
#include "connectionpool.h"
int main()
{
	cout << sizeof(long) << endl;
	//// 十线程 10000数据： 6378ms  --  2681
	//Connection conn;
	//conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//this_thread::sleep_for(chrono::milliseconds(1000));

	//clock_t startTime = clock();

	//thread t1([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//});
	//thread t2([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t3([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t4([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//	/*	ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t5([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//	/*	ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t6([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t7([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t8([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t9([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//	/*	ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//thread t10([]() {
	//	for (int i = 0; i < 1000; i++) {
	//		Connection conn;
	//		conn.connect("127.0.0.1", 3306, "root", "root", "chat");
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		conn.update(sql);
	//		/*ConnectionPool* pool = ConnectionPool::getConnectionPool();
	//		shared_ptr<Connection> p = pool->getConnection();
	//		char sql[512]{ 0 };
	//		sprintf_s(sql, "insert into user(name,age,sex) values('%s','%d','%s')", "zhang san", 20, "male");
	//		p->update(sql);*/
	//	}
	//	});
	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
	//t5.join();
	//t6.join();
	//t7.join();
	//t8.join();
	//t9.join();
	//t10.join();
	//cout << (clock() - startTime) << "ms" << endl;
	return 0;
}