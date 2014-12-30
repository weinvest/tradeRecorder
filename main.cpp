#include <iostream>
#include "BrokerAccountManager.h"
#include "CTPMarket.h"
#include "FileHandler.h"
using namespace std;

int main()
{
    auto logConfRe = Wildcard2Regex("conf/*.log");
    PathVec logConfPaths;
    FindFiles("conf",logConfRe,logConfPaths);

    for(auto& logPath : logConfPaths)
    {
	LoadLogConf(logPath.string());
    }

    BrokerAccountManager accounts;
    accounts.Load("conf/Account.json");
    if(0 == accounts.Accounts().size())
    {
        std::cout<<"load account failed"<<std::endl;
        exit(-1);
    }

    CTPMarket market;
    market.Initialize(accounts.Accounts().front());
    market.Connect();
    market.Join();
    return 0;
}

