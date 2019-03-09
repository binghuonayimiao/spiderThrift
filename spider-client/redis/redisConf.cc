#include "redisConf.h"
#include <stdlib.h>
#include "../thirdparty/json/json.h"
#include <string>
#include <iostream>
#include <fstream>

namespace ccx{

using std::ifstream;
using std::cout;
using std::endl;

RedisConf::RedisConf()
{
    getConf();
}

void RedisConf::getConf()
{
    ifstream ifs;
    ifs.open("../redis/redisConf.json");
    if(!ifs.good())
    {
        //cout << "open RedisConf.json error" << endl;
        //exit(EXIT_FAILURE);
        ifs.open("./redis/redisConf.json");
    }

    Json::Value root;
    Json::Reader reader;
    if(!reader.parse(ifs, root, false))
    {
        LOG_DEBUG << "RedisConf json reader error";
        exit(EXIT_FAILURE);
    }
    
    _ip = root["IP"].asString();
    _port = root["PORT"].asInt();
    ifs.close();
}

string RedisConf::getIP()
{
    return _ip;
}

int RedisConf::getPort()
{
    return _port;
}

}
