
#include "BrokerAccount.h"
#include <algorithm>

BrokerAccount::BrokerAccount(const string & brokerId,const string & userId,const string & passwd)
{
    mBrokerId = brokerId;
    mUserId = userId;
    mPasswd = passwd;
}

const string & BrokerAccount::GetBrokerId()
{
    return mBrokerId;
}

const string & BrokerAccount::GetUserId()
{
    return this->mUserId;
}

const string & BrokerAccount::GetPassword()
{
    return this->mPasswd;
}

void BrokerAccount::AddFeedServer(const string & serverAddress)
{
    if(find(mFeedServers.begin(),mFeedServers.end(),serverAddress) == mFeedServers.end())
    {
        mFeedServers.push_back(serverAddress);
    }
}

void BrokerAccount::AddFeedServer(const string ip, const int port)
{
    mFeedServerInfos.push_back(ServerInfo(ip, port));
}

void BrokerAccount::AddTradeServer(const string & serverAddress)
{
    if(find(mTradeServers.begin(),mTradeServers.end(),serverAddress) == mTradeServers.end())
    {
        mTradeServers.push_back(serverAddress);
    }
}

const std::vector<string> & BrokerAccount::GetFeedServers()
{
    return mFeedServers;
}

const std::vector<string> & BrokerAccount::GetTradeServers()
{
    return mTradeServers;
}
