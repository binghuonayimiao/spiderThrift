#include <iostream>
#include "redis.h"
#include "spiderHandler.h"
#include "../thirdparty/boost/threadpool.hpp"
#include "MyLog.h"
#include "../thrift/gen-cpp/MySever.cpp"
using namespace std;

int main(int argc, char**argv)
{
    isFirstWrite = true;
	MyLog::Init("log");
	LOG_DEBUG<<"-----------------------server start -----------------------";
    thriftStart();
    return 0;
    
}
