#ifndef CTPMARKET_H
#define CTPMARKET_H
#include <string>
#include "DLog.h"
#include "ThostFtdcTraderApi.h"
#include "BrokerAccount.h"

class CTPMarket : public CThostFtdcTraderSpi
{
public:
    CTPMarket();
    ~CTPMarket();
    void Connect();
    void Disconnect();
    void Join();
    void Release();
    void Initialize(BrokerAccount::Ptr pAccount);
private:
    void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
    void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField * pInvestorPosition, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast);
    void OnRspQryTradingAccount(CThostFtdcTradingAccountField * pTradingAccount, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast);
    void OnRtnOrder(CThostFtdcOrderField *pOrder) ;
    void OnRtnTrade(CThostFtdcTradeField *pTrade);
    void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
    void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
    void OnHeartBeatWarning(int nTimeLapse);
    void OnFrontConnected();
    void OnFrontDisconnected(int nReason);
    void OnRspUserLogin(CThostFtdcRspUserLoginField * pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
private:
    bool IsErrorRspInfo(CThostFtdcRspInfoField * pRspInfo);
    void OutputTrade(CThostFtdcTradeField *pTrade);
    void OutputOrder(CThostFtdcOrderField *pOrder);
    void ReqUserLogin();
    void ReqSettlementInfoConfirm();
    BrokerAccount::Ptr GetAccount() { return mAccount; }
    static void BlockSignal();
private:
    CThostFtdcTraderApi *mCtpTraderApi;
    int mRequestId;
    int mFrontId;
    int mSessionId;
    bool mIsFrontConnected;
    Logger mLog;
    BrokerAccount::Ptr mAccount;
};

#endif // CTPMARKET_H
