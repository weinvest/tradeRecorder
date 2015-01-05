#include "CTPMarket.h"
#include <boost/thread/once.hpp>
#include "DLog.h"
#include <memory>
#include <cstring>
#include <signal.h>


CTPMarket::CTPMarket()
    :mRequestId(0)
    ,mFrontId(0)
    ,mSessionId(0)
    ,mIsFrontConnected(false)
    ,mLog(Logger::getInstance("Market"))
{
    mIsFrontConnected = false;
    mFrontId = 0;
    mSessionId = 0;
    static boost::once_flag init_instance = BOOST_ONCE_INIT;
    boost::call_once(init_instance,&CTPMarket::BlockSignal);
}


CTPMarket::~CTPMarket()
{
    Release();
}

void CTPMarket::Connect()
{
    INFO(mLog,mAccount->GetUserId() << "market begin to connect");
    mCtpTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("TD");
    mCtpTraderApi->RegisterSpi(this);
    mRequestId = 0;
    auto tradeServers = GetAccount()->GetTradeServers();
    for(string server : tradeServers)
    {
        mCtpTraderApi->RegisterFront(const_cast<char*>(server.c_str()));
    }
    mCtpTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);
    mCtpTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);

    mCtpTraderApi->Init();
}

void CTPMarket::Disconnect()
{
    Release();
    INFO(mLog,mAccount->GetUserId() << " disconnected.");
}

void CTPMarket::Initialize(BrokerAccount::Ptr pAccount)
{
    mAccount = pAccount;
}

void CTPMarket::OnRspError(CThostFtdcRspInfoField *pRspInfo, int /*nRequestID*/, bool /*bIsLast*/)
{
    IsErrorRspInfo(pRspInfo);
}

void CTPMarket::OnRspOrderAction(CThostFtdcInputOrderActionField *pOrder, CThostFtdcRspInfoField *pRspInfo, int /*nRequestID*/, bool /*bIsLast*/)
{
    std::string funcName = "OnRspOrderAction(";
    INFO(mLog,funcName<<pRspInfo->ErrorID<<","<<pRspInfo->ErrorMsg<<")BrokerID:"<<pOrder->BrokerID
    <<",InvestorID:"<<pOrder->InvestorID
    <<",OrderActionRef:"<<pOrder->OrderActionRef
    <<",OrderRef:"<<pOrder->OrderRef
    <<",RequestID:"<<pOrder->RequestID
    <<",FrontID:"<<pOrder->FrontID
    <<",SessionID:"<<pOrder->SessionID
    <<",ExchangeID:"<<pOrder->ExchangeID
    <<",OrderSysID:"<<pOrder->OrderSysID
    <<",ActionFlag:"<<pOrder->ActionFlag
    <<",LimitPrice:"<<pOrder->LimitPrice
    <<",VolumeChange:"<<pOrder->VolumeChange
    <<",UserID:"<<pOrder->UserID
    <<",InstrumentID:"<<pOrder->InstrumentID);
}

void CTPMarket::OnRspOrderInsert(CThostFtdcInputOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID,bool /*bIsLast*/)
{
    std::string funcName = "OnRspOrderInsert(";
    if(-11 == nRequestID)
    {
        funcName = "OnErrRtnOrderInsert(";
    }
    INFO(mLog,funcName<<pRspInfo->ErrorID<<","<<pRspInfo->ErrorMsg<<"):BrokerID:"<<pOrder->BrokerID
    <<",InvestorID:"<<pOrder->InvestorID
    <<",InstrumentID:"<<pOrder->InstrumentID
    <<",OrderRef:"<<pOrder->OrderRef
    <<",UserID:"<<pOrder->UserID
    <<",OrderPriceType:"<<pOrder->OrderPriceType
    <<",Direction:"<<pOrder->Direction
    <<",CombOffsetFlag:"<<pOrder->CombOffsetFlag
    <<",CombHedgeFlag:"<<pOrder->CombHedgeFlag
    <<",LimitPrice:"<<pOrder->LimitPrice
    <<",VolumeTotalOriginal:"<<pOrder->VolumeTotalOriginal
    <<",TimeCondition:"<<pOrder->TimeCondition
    <<",GTDDate:"<<pOrder->GTDDate
    <<",VolumeCondition:"<<pOrder->VolumeCondition
    <<",MinVolume:"<<pOrder->MinVolume
    <<",ContingentCondition:"<<pOrder->ContingentCondition
    <<",StopPrice:"<<pOrder->StopPrice
    <<",ForceCloseReason:"<<pOrder->ForceCloseReason
    <<",IsAutoSuspend:"<<pOrder->IsAutoSuspend
    <<",BusinessUnit:"<<pOrder->BusinessUnit
    <<",RequestID:"<<pOrder->RequestID
    <<",UserForceClose:"<<pOrder->UserForceClose
    <<",IsSwapOrder:"<<pOrder->IsSwapOrder);

}

void CTPMarket::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField */*pSettlementInfoConfirm*/, CThostFtdcRspInfoField *pRspInfo, int /*nRequestID*/, bool bIsLast)
{
    if(!IsErrorRspInfo(pRspInfo))
    {
        if(bIsLast)
        {
            INFO(mLog,mAccount->GetUserId() << " settlement confirm success.");
        }
    }
    else
    {
        WARN(mLog,mAccount->GetUserId() << " settlement confirm failed.Reason is  " << pRspInfo->ErrorMsg);
    }
}

void CTPMarket::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField * /*pInvestorPosition*/, CThostFtdcRspInfoField * /*pRspInfo*/, int /*nRequestID*/, bool /*bIsLast*/)
{

}

void CTPMarket::OnRspQryTradingAccount(CThostFtdcTradingAccountField * /*pTradingAccount*/, CThostFtdcRspInfoField * /*pRspInfo*/, int /*nRequestID*/, bool /*bIsLast*/)
{

}

void CTPMarket::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    if(nullptr != pOrder)
    {
        INFO(mLog,"OnRtnOrder---------BrokerID:"<<pOrder->BrokerID
        <<",InvestorID:"<<pOrder->InvestorID
        <<",InstrumentID:"<<pOrder->InstrumentID
        <<",OrderRef:"<<pOrder->OrderRef
        <<",UserID:"<<pOrder->UserID
        <<",OrderPriceType:"<<pOrder->OrderPriceType
        <<",Direction:"<<pOrder->Direction
        <<",CombOffsetFlag:"<<pOrder->CombOffsetFlag
        <<",CombHedgeFlag:"<<pOrder->CombHedgeFlag
        <<",LimitPrice:"<<pOrder->LimitPrice
        <<",VolumeTotalOriginal:"<<pOrder->VolumeTotalOriginal
        <<",TimeCondition:"<<pOrder->TimeCondition
        <<",GTDDate:"<<pOrder->GTDDate
        <<",VolumeCondition:"<<pOrder->VolumeCondition
        <<",MinVolume:"<<pOrder->MinVolume
        <<",ContingentCondition:"<<pOrder->ContingentCondition
        <<",StopPrice:"<<pOrder->StopPrice
        <<",ForceCloseReason:"<<pOrder->ForceCloseReason
        <<",IsAutoSuspend:"<<pOrder->IsAutoSuspend
        <<",BusinessUnit:"<<pOrder->BusinessUnit
        <<",RequestID:"<<pOrder->RequestID
        <<",OrderLocalID:"<<pOrder->OrderLocalID
        <<",ExchangeID:"<<pOrder->ExchangeID
        <<",ParticipantID:"<<pOrder->ParticipantID
        <<",ClientID:"<<pOrder->ClientID
        <<",ExchangeInstID:"<<pOrder->ExchangeInstID
        <<",TraderID:"<<pOrder->TraderID
        <<",InstallID:"<<pOrder->InstallID
        <<",OrderSubmitStatus:"<<pOrder->OrderSubmitStatus
        <<",NotifySequence:"<<pOrder->NotifySequence
        <<",TradingDay:"<<pOrder->TradingDay
        <<",SettlementID:"<<pOrder->SettlementID
        <<",OrderSysID:"<<pOrder->OrderSysID
        <<",OrderSource:"<<pOrder->OrderSource
        <<",OrderStatus:"<<pOrder->OrderStatus
        <<",OrderType:"<<pOrder->OrderType
        <<",VolumeTraded:"<<pOrder->VolumeTraded
        <<",VolumeTotal:"<<pOrder->VolumeTotal
        <<",InsertDate:"<<pOrder->InsertDate
        <<",InsertTime:"<<pOrder->InsertTime
        <<",ActiveTime:"<<pOrder->ActiveTime
        <<",SuspendTime:"<<pOrder->SuspendTime
        <<",UpdateTime:"<<pOrder->UpdateTime
        <<",CancelTime:"<<pOrder->CancelTime
        <<",ActiveTraderID:"<<pOrder->ActiveTraderID
        <<",ClearingPartID:"<<pOrder->ClearingPartID
        <<",SequenceNo:"<<pOrder->SequenceNo
        <<",FrontID:"<<pOrder->FrontID
        <<",SessionID:"<<pOrder->SessionID
        <<",UserProductInfo:"<<pOrder->UserProductInfo
        <<",StatusMsg:"<<pOrder->StatusMsg
        <<",UserForceClose:"<<pOrder->UserForceClose
        <<",ActiveUserID:"<<pOrder->ActiveUserID
        <<",BrokerOrderSeq:"<<pOrder->BrokerOrderSeq
        <<",RelativeOrderSysID:"<<pOrder->RelativeOrderSysID
        <<",ZCETotalTradedVolume:"<<pOrder->ZCETotalTradedVolume
        <<",IsSwapOrder:"<<pOrder->IsSwapOrder);
    }
}

void CTPMarket::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    if(nullptr != pTrade)
    {
        INFO(mLog,"OnRtnTrade--------BrokerID:" << pTrade->BrokerID
            <<",InvestorID:" <<  pTrade->InvestorID
            <<",UserID:" << pTrade->UserID
            <<",InstrumentID:"<<pTrade->InstrumentID
            <<",OrderRef:"<<pTrade->OrderRef
            <<",ExchangeID:"<<pTrade->ExchangeID
            <<",TradeID:"<<pTrade->TradeID
            <<",Direction:"<<pTrade->Direction
            <<",OrderSysID:"<<pTrade->OrderSysID
            <<",ParticipantID:"<<pTrade->ParticipantID
            <<",ClientID:"<<pTrade->ClientID
            <<",TradingRole:"<<pTrade->TradingRole
            <<",ExchangeInstID:"<<pTrade->ExchangeInstID
            <<",OffsetFlag:"<<pTrade->OffsetFlag
            <<",HedgeFlag:"<<pTrade->HedgeFlag
            <<",Price:"<<pTrade->Price
            <<",Volume:"<<pTrade->Volume
            <<",TradeDate:"<<pTrade->TradeDate
            <<",TradeTime:"<<pTrade->TradeTime
            <<",TradeType:"<<pTrade->TradeType
            <<",PriceSource:"<<pTrade->PriceSource
            <<",TraderID:"<<pTrade->TraderID
            <<",OrderLocalID:"<<pTrade->OrderLocalID
            <<",ClearingPartID:"<<pTrade->ClearingPartID
            <<",BusinessUnit:"<<pTrade->BusinessUnit
            <<",SequenceNo:"<<pTrade->SequenceNo
            <<",TradingDay:"<<pTrade->TradingDay
            <<",SettlementID:"<<pTrade->SettlementID
            <<",BrokerOrderSeq:"<<pTrade->BrokerOrderSeq
            <<",TradeSource:"<<pTrade->TradeSource);
    }
}

void CTPMarket::OnErrRtnOrderInsert(CThostFtdcInputOrderField * pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    OnRspOrderInsert(pInputOrder,pRspInfo,-11,true);
}

void CTPMarket::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    INFO(mLog,"OnErrRtnOrderAction(" <<pRspInfo->ErrorID<<","<<pRspInfo->ErrorMsg<<")"
        <<",BrokerID:"<<pOrderAction->BrokerID
        <<",InvestorID:"<<pOrderAction->InvestorID
        <<",OrderActionRef:"<<pOrderAction->OrderActionRef
        <<",OrderRef:"<<pOrderAction->OrderRef
        <<",RequestID:"<<pOrderAction->RequestID
        <<",FrontID:"<<pOrderAction->FrontID
        <<",SessionID:"<<pOrderAction->SessionID
        <<",ExchangeID:"<<pOrderAction->ExchangeID
        <<",OrderSysID:"<<pOrderAction->OrderSysID
        <<",ActionFlag:"<<pOrderAction->ActionFlag
        <<",LimitPrice:"<<pOrderAction->LimitPrice
        <<",VolumeChange:"<<pOrderAction->VolumeChange
        <<",ActionDate:"<<pOrderAction->ActionDate
        <<",ActionTime:"<<pOrderAction->ActionTime
        <<",TraderID:"<<pOrderAction->TraderID
        <<",InstallID:"<<pOrderAction->InstallID
        <<",OrderLocalID:"<<pOrderAction->OrderLocalID
        <<",ActionLocalID:"<<pOrderAction->ActionLocalID
        <<",ParticipantID:"<<pOrderAction->ParticipantID
        <<",ClientID:"<<pOrderAction->ClientID
        <<",BusinessUnit:"<<pOrderAction->BusinessUnit
        <<",OrderActionStatus:"<<pOrderAction->OrderActionStatus
        <<",UserID:"<<pOrderAction->UserID
        <<",StatusMsg:"<<pOrderAction->StatusMsg
        <<",InstrumentID:"<<pOrderAction->InstrumentID);
}

void CTPMarket::OnHeartBeatWarning(int /*nTimeLapse*/)
{

}

void CTPMarket::OnFrontConnected()
{
    INFO(mLog,mAccount->GetUserId() << " connected");
    mIsFrontConnected = true;
    ReqUserLogin();
}

void CTPMarket::OnFrontDisconnected(int nReason)
{
    INFO(mLog,mAccount->GetUserId() << " disconnected,reason is " << nReason);
    mCtpTraderApi->SubscribePublicTopic(THOST_TERT_RESUME);
    mCtpTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
    mIsFrontConnected = false;
}

void CTPMarket::OnRspUserLogin(CThostFtdcRspUserLoginField * pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int /*nRequestID*/, bool bIsLast)
{
    if(!IsErrorRspInfo(pRspInfo) && bIsLast)
    {
        INFO(mLog,mAccount->GetUserId() << " login successfully.");
        mFrontId = pRspUserLogin->FrontID;
        mSessionId = pRspUserLogin->SessionID;
        ReqSettlementInfoConfirm();
    }
}

void CTPMarket::OnRspUserLogout(CThostFtdcUserLogoutField */*pUserLogout*/, CThostFtdcRspInfoField *pRspInfo, int /*nRequestID*/, bool /*bIsLast*/)
{
    INFO(mLog,mAccount->GetUserId() << " logout " << pRspInfo->ErrorMsg);
}

void CTPMarket::Release()
{
    if(mCtpTraderApi != nullptr)
    {
        mCtpTraderApi->RegisterSpi(nullptr);
        mCtpTraderApi->Release();
        mCtpTraderApi = nullptr;
    }
}

void CTPMarket::Join()
{
    if(nullptr != mCtpTraderApi)
    {
        mCtpTraderApi->Join();
    }
}

bool CTPMarket::IsErrorRspInfo(CThostFtdcRspInfoField * pRspInfo)
{
    bool bResult = (nullptr != pRspInfo) && (pRspInfo->ErrorID != 0);
    if(bResult)
    {
        WARN(mLog,mAccount->GetUserId() << " received error response : " << pRspInfo->ErrorMsg);
    }
    return bResult;
}


void CTPMarket::ReqUserLogin()
{
    {
        auto account = GetAccount();
        CThostFtdcReqUserLoginField reqUserLogin;
        std::strcpy(reqUserLogin.BrokerID, account->GetBrokerId().c_str());
        std::strcpy(reqUserLogin.UserID, account->GetUserId().c_str());
        std::strcpy(reqUserLogin.Password, account->GetPassword().c_str());
        int iResult = mCtpTraderApi->ReqUserLogin(&reqUserLogin,++mRequestId);
        INFO(mLog,mAccount->GetUserId() << " send login requst: " << ((iResult == 0) ? "Successful " : "Failed"));
    }
}


void CTPMarket::ReqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField reqSettlementInfoConfirmField;
    memset(&reqSettlementInfoConfirmField,0,sizeof(reqSettlementInfoConfirmField));
    auto account = GetAccount();
    std::strcpy(reqSettlementInfoConfirmField.BrokerID,account->GetBrokerId().c_str());
    std::strcpy(reqSettlementInfoConfirmField.InvestorID,account->GetUserId().c_str());
    int iResult = mCtpTraderApi->ReqSettlementInfoConfirm(&reqSettlementInfoConfirmField, ++mRequestId);
    INFO(mLog,mAccount->GetUserId() << " send settlement request: " << ((iResult == 0) ? "Successful" : "Failed"));
}

void CTPMarket::BlockSignal()
{
#ifndef _WIN32
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block,SIGUSR1);
    sigprocmask(SIG_BLOCK, &block, NULL);
#endif
}
