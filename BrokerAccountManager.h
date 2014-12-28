#ifndef BROKERACCOUNTMANAGER_H
#define BROKERACCOUNTMANAGER_H
#include <istream>
#include "DLog.h"
#include "BrokerAccount.h"

class BrokerAccountManager
{
public:
    BrokerAccountManager();
    bool Load(const std::string& path);
    bool Load(const std::string&,std::istream& accountStream);

    void Add(BrokerAccount::Ptr pAccount) { mBrokerAccounts.push_back(pAccount); }

    const std::vector<BrokerAccount::Ptr>& Accounts() const { return mBrokerAccounts; }
private:
    Logger mLog;
    std::vector<BrokerAccount::Ptr> mBrokerAccounts;
};

#endif // BROKERACCOUNTMANAGER_H
