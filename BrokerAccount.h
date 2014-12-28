#ifndef BROKERACCOUNT_H
#define BROKERACCOUNT_H

#include <boost/smart_ptr.hpp>
#include <string>
#include <vector>
using namespace std;

struct ServerInfo
{
    ServerInfo(const std::string& ip, const int port)
        : IP(ip), Port(port){}
    std::string IP;
    int Port;
};

class BrokerAccount
{
public:
    typedef boost::shared_ptr<BrokerAccount> Ptr;
    BrokerAccount(const string & brokerId,const string & userId,const string & passwd);
    void SetName(const std::string& name) { mName = name; }
    const std::string& GetName() const { return mName; }
    const string & GetBrokerId();
    const string & GetUserId();
    const string & GetPassword();
    void AddFeedServer(const string & serverAddress);
    void AddFeedServer(const std::string ip, const int port);
    void AddTradeServer(const string & serverAddress);
    const std::vector<string> & GetFeedServers();
    const std::vector<string> & GetTradeServers();
    const std::vector<ServerInfo> & GetFeedServerInfos() { return mFeedServerInfos; }
private:
    std::string mName;
    string mBrokerId;
    string mUserId;
    string mPasswd;
    std::vector<string> mFeedServers;
    std::vector<ServerInfo> mFeedServerInfos;
    std::vector<string> mTradeServers;
};

#endif // BROKERACCOUNT_H
