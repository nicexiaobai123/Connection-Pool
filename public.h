#pragma once
#include <mysql.h>
#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
#include <ctime>
using namespace std;
#define LOG(str) \
		cout << __FILE__ << ":" << __LINE__ << " " \
		__TIMESTAMP__ << ":" << str << endl;
//#pragma comment(lib,"libmysql.lib")