#ifndef __REDIS_H__
#define __REDIS_H__

#include "redisConf.h"

#include "../thirdparty/hiredis/hiredis.h"
#include "../src/MyLog.h"

namespace ccx{
using std::string;

class Redis
{
    public:
        Redis();
    public:
        void Connect();
        void disConnect();
    public:
        void setString(const string & key, const string & value);
        void setString(const string & key, const int & value);
        void setString(const string & key, const float & value);
        void setHash(const string & key, const string & member, const string & value);
        bool isHashMember(const string & key, const string & member);
    private:
        void setString(const string & data);
    public:
        void getString(const string & key, string & value);
        void getString(const string & key, int & value);
        void getString(const string & key, float & value);
    private:
        void getString(const string & key);
    private:
        void freeReply();
        bool isError();
    private:
        RedisConf _conf;
        redisContext * _context;
        redisReply * _reply;
};
}

#endif
