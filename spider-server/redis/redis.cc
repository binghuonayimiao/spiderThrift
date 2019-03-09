#include "redis.h"

#include <string.h>
#include <stdlib.h>

#include <sstream>
#include <iostream>

namespace ccx{

using std::cout;
using std::endl;
using std::stringstream;
using std::string;

#define SETSTRING(key, value) \
    stringstream ss;\
    ss << "SADD " << key << " " << value;\
    string s;\
    getline(ss, s);\
    setString(s);

#define SETHASH(key, member, value) \
    stringstream ss;\
    ss << "HSET " << key << " " << member << " " << value;\
    string s;\
    getline(ss, s);\
    setString(s);

Redis::Redis()
: _conf(),_reply(NULL),_context(NULL)
{
}

void Redis::Connect()
{
    _context = ::redisConnect(_conf.getIP().c_str(), _conf.getPort());
    LOG_DEBUG<< _conf.getIP() << "-" << _conf.getPort();
    if(_context && _context->err)
    {
        LOG_DEBUG << "connect redis error";
        exit(EXIT_FAILURE);    
    }
    LOG_DEBUG << "redis Connect success";
}

void Redis::disConnect()
{
    ::redisFree(_context);
    LOG_DEBUG << "redis disConnect success";
}

void Redis::setString(const string & data)
{
    freeReply();
    _reply = (redisReply*)::redisCommand(_context, data.c_str());
    if(!isError())
    {
        if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK") == 0))
        {
            LOG_DEBUG << "Failed to execute SET(string)";
        }    
    }
}

void Redis::setHash(const string & key, const string & member, const string & value){
    SETHASH(key, member, value);
}
void Redis::setString(const string & key, const string & value)
{
    SETSTRING(key, value);
    //string s = "SET " + key + " " + value;
    //setString(s);
}

void Redis::setString(const string & key, const int & value)
{
    SETSTRING(key, value);
}

void Redis::setString(const string & key, const float & value)
{
    SETSTRING(key, value);
}

void Redis::getString(const string & key)
{
    freeReply();
    _reply = (redisReply*)::redisCommand(_context, "GET %s", key.c_str());
}

bool Redis::isHashMember(const string & key, const string & member)
{

	freeReply();
    string s = "hexists " + key + " " + member;
    _reply = (redisReply*)::redisCommand(_context, s.c_str());
	
    int res = 0;
    if(!isError() && _reply->type == REDIS_REPLY_INTEGER)
    {
        res = (int)(_reply->integer);
    }
    return 1==res ? true:false;
}

void Redis::getString(const string & key, string & value)
{
    getString(key);
    if(!isError() && _reply->type == REDIS_REPLY_STRING)
    {
        value = _reply->str;
    }
}

void Redis::getString(const string & key, int & value)
{
    getString(key);
    if(!isError() && _reply->type == REDIS_REPLY_STRING)
    {
        value = ::atoi(_reply->str);
    }
}

void Redis::getString(const string & key, float & value)
{
    getString(key);
    if(!isError() && _reply->type == REDIS_REPLY_STRING)
    {
        value = ::atof(_reply->str);
    }
}

void Redis::freeReply()
{
    if(_reply)
    {
        ::freeReplyObject(_reply);
        _reply = NULL;
    }
}

bool Redis::isError()
{
    if(NULL == _reply)
    {
        freeReply();
        disConnect();
        Connect();
        return true;
    }
    return false;
}

}
