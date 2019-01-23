/*
 *
 *  Created on: 2015年4月10日
 *      Author: jouvechen
 */

#ifndef MID_KAFKA_H_
#define MID_KAFKA_H_


#include <string>
#include <sstream>
#include "FZConfig.h"
#include "MsgWithID.h"
#include "util/tc_thread_pool.h"
#include <jmem/jmem_hashmap.h>

#include "util/tc_md5.h"
#include "util/tc_common.h"
#include "util/tc_timeprovider.h"
#include "util/tc_atomic.h"

#include "servant/Application.h"
#include <servant/Communicator.h>
#include <servant/CommunicatorFactory.h>
#include "promise/promise.h"
#include "servant/PropertyReport.h"

#include "SagentServant.h"


using namespace std;
using namespace taf;
using namespace mid;
using namespace MsgWithID;

namespace mid
{

class MiddleLogger;
/**
* 用于生产和消费线程间传递信息
*/
class MsgPackPtr :public TC_HandleBase
{
public:
	MsgPackPtr(int64_t timeIn,int size, int numOfLogs)
	{
		_packMsgTime = timeIn;
		_packSize = size;
		_logNum = numOfLogs;
	}
	//inPack的值除了msgid外都要初始化完成
	MsgWithID::MsgPack _inPack;

    //这里加入字段是为了防止前端不给inPack字段赋值
	//对应msgpack的logTime
	int64_t _packMsgTime;
	//对应msgpack的	msgSize
	int _packSize;
	//对应msgpack的	logNum
	int _logNum;	
};

typedef taf::TC_AutoPtr<MsgPackPtr> MsgPackPtrT;
typedef taf::TC_AutoPtr<MiddleLogger> MiddleLoggerPtrT;

/**
* msgpack 和logger集合类
*/
class LoggerWithPack :public TC_HandleBase
{
public:
	LoggerWithPack(const MiddleLoggerPtrT & logger, const MsgPackPtrT & packPtr)
	{
		_loggerPtr = logger; 
		_msgPackPtr = packPtr;
	}

	MiddleLoggerPtrT _loggerPtr;
    MsgPackPtrT _msgPackPtr;
};

typedef taf::TC_AutoPtr<LoggerWithPack> LoggerWithPackPtrT;


/**
* Agent写入对象
*
*/
class KafkaWrite:public TC_HandleBase
{
	public:
		KafkaWrite();
		KafkaWrite(bool isSync);
		~KafkaWrite();

		typedef SAGENT::SagentServantPrxCallbackPromise::PromiseloggerPtr LoggerFutureResultPtr;
        typedef promise::Future< SAGENT::SagentServantPrxCallbackPromise::PromiseloggerPtr> LoggerFuture;
        /**
        * 初始化。isBackup决定本实例是否是向backup发送
        * 
        */
		void init(const string& endpoints, bool isBackup);
		string getEndpoints();
        /**
        * 异步写Agent。这里做压缩
        * 
        */		
		int writeToAgentWithPtr(LoggerWithPackPtrT taskPtr);		
        /**
        * 异步写Agent.写Default失败会转到bakcup.写backup失败会丢弃
        *
        */		
		int writeInnerAgent(LoggerWithPackPtrT taskPtr);		
        /**
        * 异步写Backup Agent
        *
        */		
		int backupRewriteToAgent(LoggerWithPackPtrT taskPtr);		

	private:
		SAGENT::SagentServantPrx _mTafProxy;
		//obj name
		string _eps;
		//GZ压缩等级
		int _gzCompLevel;
		//是否备用Agent
		bool _isBackup;
};

typedef taf::TC_AutoPtr<KafkaWrite> KafkaWritePtrT;

/**
* 管理Agent写对象:
* 1. QOS
* 2. 管理Default及Backup Writer
*/
class KafkaWriteManager:
		public TC_Singleton<KafkaWriteManager, CreateUsingNew, DefaultLifetime>
{
public:
    KafkaWriteManager();
    ~KafkaWriteManager();
	int setDefaultProxy(const string & prx);
    int setBackupProxy(const string & prx);
    void reSendToBackWithPtr(const LoggerWithPackPtrT& taskPtr, const string & expStr );

	KafkaWritePtrT getDefaultProxy();
	KafkaWritePtrT getBackupProxy();	
    /**
    *  获得写对象。会根据QOS结果获得default或者backup agent
    *
    */
	KafkaWritePtrT getKfkProxy(const string &asf);
    /**
    *  流量迁移
    *
    */	
    void toBackup();
    void toDefault();
    bool getUsingBackup();
    /**
    *  QOS线程
    *
    */    
    void runQos();
    void startQosThread();    
    /**
    * QOS 统计值
    *
    */    
    void incDefaultSucc();
    void incDefaultSend();    
    void incDefaultRetrySucc();        
    /**
    *  QOS时间间隔
    *
    */    
    void setQosStatInterval(unsigned int intval);
private:
	volatile bool _usingBackup;
  	KafkaWritePtrT _defaultProxy;    
  	KafkaWritePtrT _backupProxy;      	
  	TC_ThreadPool   _statPool;
	taf::TC_ThreadQueue<int> _statPoolTimerQueue;  
	
  	TC_Atomic _dafaultSuccNum;
  	TC_Atomic _totalDefaultSendNum;  	
  	TC_Atomic _dafaultRetrySuccNum;  	
  	unsigned int _qosInterval;
  	unsigned int _changingThrd;
    TC_Atomic _retryInterval;

    bool _terminated;
	std::map<string,KafkaWritePtrT> _asfConf;  		
};
};

#endif /* MID_KAFKA_H_ */
