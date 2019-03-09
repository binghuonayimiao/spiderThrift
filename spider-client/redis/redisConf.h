#ifndef __REDISCONF_H__
#define __REDISCONF_H__
#include <string>
#include "../src/MyLog.h"
namespace ccx{
using std::string;
class RedisConf
{
    public:
        RedisConf();
        void getConf();
        string getIP();
        int getPort();
    private:
        string _ip;
        int _port;
};
}
#endif