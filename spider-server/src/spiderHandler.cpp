#include "spiderHandler.h"

bool isFirstWrite;

void init(){
    
}

void task(string url){
    ccx::Redis redis;
    redis.Connect();

    int socketFd = getSocketFd(url);
    vector<string> profileIDVec;
    setProfileIDVec(profileIDVec, url, socketFd);
    getInfoAndWriteToRedis(profileIDVec, redis, socketFd);

    redis.disConnect();
}
void GetUrlAndPath(const string url, string &HostUrl, string &PagePath)
{
    HostUrl = url;
    PagePath = "/";
    //去除 http:// 字符串
    int pos = HostUrl.find("http://");
    if(pos != -1)
    {
        HostUrl = HostUrl.replace(pos, 7, "");
    }
 
    //去除 https:// 字符串
    pos = HostUrl.find("https://");
    if(pos != -1)
    {
        HostUrl = HostUrl.replace(pos, 8, "");
    }
 
    //提取主机端口和路径
    pos = HostUrl.find("/");
    if(pos != -1)
    {
        PagePath = HostUrl.substr(pos);
        HostUrl = HostUrl.substr(0, pos);
    }
}
int getSocketFd(const string url){
    //记录主机的信息，包括主机名、别名、地址类型、地址长度和地址列表
    struct hostent *host;
    string HostUrl, PagePath;
 
    //从url获取主机url和路径
    GetUrlAndPath(url, HostUrl, PagePath);
 
    //通过主机名连接主机
    host = gethostbyname(HostUrl.c_str());
    if(host == 0)
    {
        LOG_DEBUG<<"gethostbyname error";
        exit(1);
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = ((struct in_addr*) (host->h_addr))->s_addr;
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd == -1)
    {
        LOG_DEBUG<<"create socketFd error";
        exit(1);
    }
    int ret = connect(socketFd, (const sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        LOG_DEBUG<<"connect error";
        exit(1);
    }
    return socketFd;
}
string getpagecontent(const string url, int &socketFd)
{
    //记录主机的信息，包括主机名、别名、地址类型、地址长度和地址列表
    struct hostent *host;
    string HostUrl, PagePath;
 
    //从url获取主机url和路径
    GetUrlAndPath(url, HostUrl, PagePath);
 
    string requestHeader;
    requestHeader   = "GET " + PagePath + " HTTP/1.1\r\n";
    requestHeader += "Host: " + HostUrl + "\r\n";
    requestHeader += "Accept: */*\r\n";
    requestHeader += "User-Agent: Mozilla/4.0(compatible)\r\n";
    requestHeader += "connection:Keep-Alive\r\n";
    requestHeader += "\r\n";

    int ret = send(socketFd, requestHeader.c_str(), requestHeader.size(), 0);
    if(ret == -1)
    {
        LOG_DEBUG<<"send error";
        exit(1);
    }
 
    struct timeval timeout = {1, 0};
    setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(struct timeval));
    char c;
    bool flag = true;
    while(recv(socketFd, &c, 1, 0))
    {
        if(c == '\r')
        {
            continue;
        }
        else if(c == '\n')
        {
            if(flag == false)
                break;
            flag = false;
        }
        else
        {
            flag = true;
        }
    }
 
    int len, BUFFERSIZE = 512;
    char buffer[BUFFERSIZE];
    string pagecontent = "";
    while((len = recv(socketFd, buffer, BUFFERSIZE-1, 0)) > 0)
    {
        buffer[len] = '\0';
        pagecontent += buffer;
    }
 
    return pagecontent;
}
vector<string> getRegularResult(string &str, regex &reg){
    vector<string> vec;
    sregex_iterator ite(str.begin(), str.end(), reg);
    sregex_iterator end;
    for(; ite != end; ++ite)
    {
        //cout << ite->str() << endl;
        vec.push_back(ite->str());
    }
    return vec;
}
void writeStrToRedis(std::string &str){
    std::ostringstream oss;
	oss << std::this_thread::get_id();
	std::string stid = oss.str();
    if(isFirstWrite){
        redis.setHash("zgs_hash", str, stid);
        isFirstWrite = false;
    }
    if(! redis.isHashMember("zgs_hash", str)){
        redis.setHash("zgs_hash", str, stid);
        LOG_DEBUG<<"name= "<< str<< "stid= " << stid;
    }
}
void getInfoAndWriteToRedis(vector<string> &profileIDVec, ccx::Redis &redis, int &socketFd){
    if(profileIDVec.empty()){
        LOG_DEBUG<<"funName = getInfo, profileIDVec is empty";
        return;
    }
    std::ostringstream oss;
	oss << std::this_thread::get_id();
	std::string stid = oss.str();

    string baseUrl = "http://www.youyuan.com/";
    for(int i = 0; i < (int)profileIDVec.size(); i++){
        string url = baseUrl + profileIDVec[i];
        sleep(1); //睡1s,体现出多线程的优势
        string responseStr = getpagecontent(url, socketFd);
        LOG_DEBUG<<"ID= "<<profileIDVec[i];
        regex nameRegxTemp("main\"><strong>[\u4e00-\u9fa5a-zA-Z0-9]{1,100}</strong><span><a href");
        vector<string> nameVecTemp = getRegularResult(responseStr, nameRegxTemp);
        if(!nameVecTemp.empty()){
            regex nameRegx("[\u4e00-\u9fa5]{1,100}");
            vector<string> nameVec = getRegularResult(nameVecTemp[0], nameRegx);

            if(!nameVec.empty()){
					if(0 == i){
                        redis.setHash("zgs_hash", nameVec[0], stid);
                    }
                    //判断该键在hash中有没有存储过，有则证明其它线程已经爬出过，本线程顺延10个继续爬取
                    if(! redis.isHashMember("zgs_hash", nameVec[0])){
                        redis.setHash("zgs_hash", nameVec[0], stid);
                        LOG_DEBUG<<"name= "<< nameVec[0]<< "stid= " << stid;
                    }else{
						LOG_DEBUG<<nameVec[0]<<"is exists";
                        i += 10;
                    }
                    
					
            }else{
                LOG_DEBUG<<"funName = getResponseInfo, nameVec is empty";
            }

        }else{
            LOG_DEBUG<<"funName = getResponseInfo, nameVecTemp is empty";
        }
        
    }

}
void setProfileIDVec(vector<string> &profileIDVec, string &baseUrl, int &socketFd){
    //爬取100页信息
    for(int i = 1; i < 10; i++){
        string url = baseUrl + "p" + to_string(i) +"/";
        string responseStr = getpagecontent(url, socketFd);
        regex reg("\\d{1,10}-profile");
        vector<string> profileIDVecTemp = getRegularResult(responseStr, reg);
		LOG_DEBUG<<"profileIDVecTemp size is"<<profileIDVecTemp.size();
        profileIDVec.insert(profileIDVec.end(), profileIDVecTemp.begin(), profileIDVecTemp.end());
    }
    
    //profileID正则匹配两个，去重处理
    profileIDVec.erase(unique(profileIDVec.begin(), profileIDVec.end()), profileIDVec.end());
    LOG_DEBUG<<"profileIDVec size is"<<profileIDVec.size();

}
