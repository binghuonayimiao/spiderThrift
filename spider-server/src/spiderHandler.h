#ifndef _SPIDERHANDLER_H
#define _SPIDERHANDLER_H

#include <iostream>
#include <string>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <unistd.h>
#include <thread>
#include "../redis/redis.h"
#include "MyLog.h"
using namespace std;
extern bool isFirstWrite;

void init();
void GetUrlAndPath(const string url, string &HostUrl, string &PagePath);
void task(string url);
int  getSocketFd(const string url);
void getInfoAndWriteToRedis(vector<string> &profileIDVec, ccx::Redis &redis, int &socketFd);
void writeStrToRedis(std::string &str);
void setProfileIDVec(vector<string> &profileIDVec, string &baseUrl, int &socketFd);
string getpagecontent(const string url, int &socketFd);
vector<string> getRegularResult(string &str, regex &reg);
#endif
