#include <boost/property_tree/json_parser.hpp>
#include "BrokerAccountManager.h"


BrokerAccountManager::BrokerAccountManager()
    :mLog(Logger::getInstance("BrokerAccount"))
{
}

bool BrokerAccountManager::Load(const std::string& path)
{
    std::ifstream brokerAccountConfig(path,std::ios_base::binary | std::ios_base::in);
    if(!brokerAccountConfig.is_open())
    {
        WARN(mLog,"Open configure file "<<path<<" failed.");
        return false;
    }
    else
    {
        return Load("",brokerAccountConfig);
    }
}


bool BrokerAccountManager::Load(const std::string&,std::istream& accountStream)
{
    using namespace boost::property_tree;
    try
    {
        ptree conf;
        read_json(accountStream,conf);
        for(const auto& accountPair : conf)
        {
            const auto& account = accountPair.second;
            std::string name = accountPair.first;
            std::string brokerId = account.get<std::string>("BrokerId");
            std::string userId = account.get<std::string>("UserId");
            std::string passwd = account.get<std::string>("Password");
            auto newBrokerAccount = boost::make_shared<BrokerAccount>(brokerId,userId,passwd);
            newBrokerAccount->SetName(name);

            for(const auto& sourceServers : account.get_child("FeedServers"))
            {
                std::string ip = sourceServers.second.get<std::string>("IP");
                std::string port = sourceServers.second.get<std::string>("Port");
                newBrokerAccount->AddFeedServer(ip + ":" + port);
                newBrokerAccount->AddFeedServer(ip, atoi(port.c_str()));
            }

            if(account.count("TradeServers"))
            {
                for(const auto& tradeServers : account.get_child("TradeServers"))
                {
                    std::string ip = tradeServers.second.get<std::string>("IP");
                    std::string port = tradeServers.second.get<std::string>("Port");
                    newBrokerAccount->AddTradeServer(ip + ":" + port);
                }
            }
            Add(newBrokerAccount);
        }

        return true;
    }
    catch(const json_parser_error& ex)
    {
        WARN(mLog,"Load broker account failed.");
    }

    return false;
}
