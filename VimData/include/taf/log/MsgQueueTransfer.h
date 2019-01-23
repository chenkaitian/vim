#ifndef __MID_LOGGER_H__
#define __MID_LOGGER_H__

#include <string>
#include <queue>
#include <map>
#include <vector>
#include <list>
#include <set>

#include "util/tc_singleton.h"
#include "util/tc_monitor.h"
#include "util/tc_thread.h"
#include "util/tc_readers_writer_data.h"
#include "util/tc_thread_queue.h"
#include "MsgWithID.h"
#include "MsgHelper.h"
#include "FZKafkaWriter.h"

#include "util/tc_thread_rwlock.h"
//#include "mid_atomic.h"

#include "log/Log.h"

using std::string;
using std::queue;
using std::map;
using std::vector;

using taf::TC_ThreadLock;
using taf::TC_Singleton;
using taf::CreateUsingNew;
using taf::DefaultLifetime;

#define MAX_APP_LEN (50)
#define MAX_SERVER_LEN (50)
#define MAX_FILE_LEN (100)

#define MAX_BUFFER_LENGTH 1024*1024*3

//60
#define STAT_INTERVAL (60)

const int64_t NO_PACK_TIME = -1;
const int FZ_THREAD_QUEUE_MAX_NUM = 25;
const int FZ_THREAD_QUEUE_DEFAULT_NUM = 8;

#define FZ_STAT_LOG_PACK_SIZE (1000)
#define FZ_STAT_LOG_RESEND_TIME (10)



namespace mid
{

#define FZ_META_FILE_NAME "/fz.groupSeq.dat" 


/**
 * @brief 统计值组合
 *
 */
class RecvAndSend
{
public:
	RecvAndSend()
	{
		_recvNum = 0;
		_sendNum = 0;		
		_rewriteSuccNum = 0;
		_refusedNum = 0;
		_asyncFailedNum = 0;		
		_rewriteFailedNum = 0;
		_oriSize = 0;
		_comprSize = 0;
	};
	int64_t _recvNum;
	int64_t _sendNum;
    /**
     * @brief 写到backup agent的成功数
     *
     */	
	int64_t _rewriteSuccNum;	
	int64_t _refusedNum;
	int64_t _asyncFailedNum;	
	int64_t _rewriteFailedNum;		
	int64_t _oriSize;	
	int64_t _comprSize;		
};

typedef std::map<int64_t,int> LogtimeStatMap;
typedef taf::TC_Functor<void,TL::TLMaker<RecvAndSend &, int>::Result> UpdaterFunc;



class StatLogInfo:public TC_HandleBase
{
public:
	StatLogInfo(int resend, vector<string> & veclogs)
	{
	    _resendTime = resend;
	    _vecStatLogs.swap(veclogs);
	}

    /**
     * 重发次数
     */
    int _resendTime;
    vector<string> _vecStatLogs;
};
typedef taf::TC_AutoPtr<StatLogInfo> StatLogInfoPtrT;



class statLogPrxCallback : public taf::LogPrxCallback
{
public:
	statLogPrxCallback(StatLogInfoPtrT pinfo)
	{
        _infoPtr = pinfo;
	}
	virtual void callback_logger(){}
	virtual void callback_loggerbyInfo();
	virtual void callback_logger_exception(taf::Int32 ret){}
    virtual void callback_loggerbyInfo_exception(taf::Int32 ret);
private:
    StatLogInfoPtrT  _infoPtr;
};


/**
 * @brief 打日志的核心类。每个ASF对应一个logger，每个logger内部有THREAD_QUEUE_NUM(25)个并发通道
 *
 */
class MiddleLogger:public TC_HandleBase {
public:
    /**
     * 
     * Param:loggerKey: ASF名称，本机ip、instid，用来上报审计，以及作为groupkey的一部分
     */	
	MiddleLogger(const Msg::MiddleLoggerKey& loggerKey);
	~MiddleLogger();
    /**
     *消费日志 
     *
     */	
	int doConsumerTaskAsfObj(LoggerWithPackPtrT taskPtr);
    /**
     * @brief 获得一个递增的seq值
     *
     */	
	int getIncLogCount();
    /**
     * 获得asf活跃group数
     *
     */
	int getAliveGroupsNum();
    /**
     * 给msgpack进行赋值，包括group & seq
     *
     */
	void preparePack(LoggerWithPackPtrT taskPtr);
	

    /**
     * 上报审计数据
     *
     */		
	RecvAndSend makeStatLogTime(const string & ip
	                            ,const string & proxyObj
                                ,vector<string> & vecStatLogV2);		
    /**
     * 设置活跃group数
     *
     */	
    void setAliveGroupsNum(int groupNum);  
    
     /**
     * 审计计数
     *
     */	   
    void addSendLogTime(int64_t logTime,int sendNum);
    void addRewriteSuccLogTime(int64_t logTime,int sendNum);    
    void addRefusedLogTime(int64_t logTime,int sendNum);	
    void addRecvLogTime(LoggerWithPackPtrT & ptr);
    void addAsyncFailedTime(int64_t logTime,int asyncFailed);
    void addRewriteFailedTime(int64_t logTime,int asyncFailed);    
    void addMemDropLogsNum(int droped);
    void addOriSizeTime(int64_t logTime,int sendNum);
    void addComprSizeTime(int64_t logTime,int sendNum);
    void addGroupDeletedNum(int dlt);

    /**
    * 审计累加，functor函数
    *
    */ 
    void updateSend(RecvAndSend & ras,int num);
    void updateRewriteSucc(RecvAndSend & ras,int num);
    void updateRewriteFailed(RecvAndSend & ras,int num);    
    void updateRecv(RecvAndSend & ras,int num);
    void updateRefuse(RecvAndSend & ras,int num);	
    void updateFailed(RecvAndSend & ras,int num);    
    void updateOriSize(RecvAndSend & ras,int num);
    void updateComprSize(RecvAndSend & ras,int num);    
    /**
    * 更新审计map
    *
    */     
    void updateAllMap(LogtimeStatMap & ltMap,
										TC_ThreadLock & ltMapMutex,
										UpdaterFunc & updater);
    /**
    * 给日志增加ip和时间头
    *
    */ 
    void addLogHead(const char *c, int len,time_t firstTime,vector<taf::Char> & outVec);
    /**
    * 通用函数，操作审计map增加审计值
    *
    */     
    void addTimedStat(int64_t logTime
                                        ,int statNum
                                        ,LogtimeStatMap & statMap
                                        ,TC_ThreadLock & mapLock);
    /**
    * 获得审计数据归属于当天哪个时间周期
    *
    */                                         
    int64_t getKeyTime(int64_t logtime);

private:

    /**
    * 获得groupid
    * @is_first 觉得是否会初始化元数据文件
    */ 
    int32_t getGroupID(int32_t groupIndex,bool is_first=true);
    /**
    * 将一条日志放入vec。需获得vec对应的锁再调用
    *
    */ 
	void addOneMsgParalNoLock(int qIdx,const char* buf,size_t buf_size, int32_t log_hashcode);

    /**
    * 将打包好的msgpack放入消费队列
    *
    */ 
	void giveToOneComsumer(LoggerWithPackPtrT & ptr, int idx);
    /**
    * 根据groupid获得seq
    *
    */ 	
	void getGidAndSeq(int & gid, int & seq) ;
    string getStatLogTime();

    int64_t getStatLogSec();
    int64_t getStatRoughMs();      
public:

    //asf标准名
  	string _asfKey;  
  	//SDK是否是以taflog模式启动。决定是否给log增加ip time头
  	string _isinTaflog;    	
  	//内存溢出条数
    TC_Atomic _memDropLogsSum;
    //元数据文件溢出次数
    TC_Atomic _groupIdxDeletedSum;

private:

    //记录上一次产生的seq。用来检查seq生成是否连续
    int volatile _lastGroupSeq[FZ_LOCAL_GROUP_MAX_NUM];

    //最大原始数据配置值
	taf::Int64 _maxPackSize;

	//存储ASF/ip/instid/md5key的对象
	Msg::MiddleLoggerKey _loggerKey;
    //临时pack，生成新msgpack时使用
	MsgWithID::MsgPack _innerPack;

	//seq生成器
	TC_Atomic _iLogCount;
    //ASF活跃group数
	volatile int _aliveGroupNum;	

	//保护getGidAndSeq
	TC_ThreadLock _mutexGidAndSeq;

	volatile int64_t _balanceNumLogTime;

    /**
    * @brief 用于统计各种数据的map和对应的锁
    *
    */	    
   	LogtimeStatMap _logTime2RecvMap;
	//protect: logTime2RecvMap 
    TC_ThreadLock _mutexlogTime2RecvMap;  
    
   	LogtimeStatMap _logTime2SendMap;   	
	//protect: logTime2SendMap 
    TC_ThreadLock _mutexlogTime2SendMap;     	

   	LogtimeStatMap _logTime2RewriteSuccMap;   	
	//protect: logTime2RewriteSuccMap 
    TC_ThreadLock _mutexlogTime2RewriteSuccMap; 
    
   	LogtimeStatMap _logTime2RefusedMap;   	
	//protect: logTime2RefusedMap 
    TC_ThreadLock _mutexlogTime2RefusedMap;     	

   	LogtimeStatMap _logTime2AsyncFailedMap;   	
	//protect: logTime2AsyncFailedMap 
    TC_ThreadLock _mutexlogTime2AsyncFailedMap;     

   	LogtimeStatMap _logTime2RewriteFailedMap;   	
	//protect: logTime2RewriteFailedMap 
    TC_ThreadLock _mutexlogTime2RewriteFailedMap;  

   	LogtimeStatMap _logTime2OriSizeMap;   	
	//protect: logTime2OriSizeMap 
    TC_ThreadLock _mutexlogTime2OriSizeMap;    

   	LogtimeStatMap _logTime2ComprSizeMap;   	
	//protect: logTime2ComprSizeMap 
    TC_ThreadLock _mutexlogTime2ComprSizeMap;    

    //汇总map        
   	std::map<int64_t,RecvAndSend> _logTime2AllMap;   	    
};

//typedef taf::TC_AutoPtr<MiddleLogger> MiddleLoggerPtrT;




//日志消费组，从队列中读取日志
class LogConsumerThreadGroup : public taf::TC_ThreadLock {
public:
	LogConsumerThreadGroup(): _terminate(false) {
		_maxQueueSize = FZConfigManager::getInstance()->getLogQueueSize();
	}

	virtual ~LogConsumerThreadGroup();

    /**
    * 启动消费线程
    *
    */ 
	void start(int iThreadNum);
	void runTask(int t_index);	
	
	void terminate();

    /**
    * 入消费队列
    *
    */	
	void dispatchOneTask(LoggerWithPackPtrT & ptr) ;

private:
	//核心数据分发队列。msgpack通过此队列传给后端发送线程
	taf::TC_ThreadQueue<LoggerWithPackPtrT> _task_queues[FZ_THREAD_QUEUE_MAX_NUM];		

	//消费线程组
	TC_ThreadPool   _tpool;
	//pack分发计数
	TC_Atomic _idForDispatch;

    bool    _terminate;
    //最大队列长度
    int _maxQueueSize;
    int _liveThreadNum;
};



/////////////////
class MsgQueueTransfer :
		public TC_Singleton<MsgQueueTransfer, CreateUsingNew, DefaultLifetime>
{
public:

	MsgQueueTransfer();
	~MsgQueueTransfer();

    /**
     * 初始化处理线程，日志处理线程，失败日志处理线程，进程标识,日志条数限制，文件根路径
     * @param threadNum, 日志处理线程数
     * @param file_path 元数据文件根路径
     */
    void init(int localPort
                ,int defaultLogs        
                ,const string& filePath //= ServerConfig::DataPath   
                ,int threadNum //= 10                       
                ,bool isTaflogMode //= false
                ,int packQueueSize //= 1000 //means:total mem use =  threadNum * packQueueSize               
                ,int maxGroup //= 1024
                ,int agentPort //= 9102
                ,bool compressGz //= true
                ,int gzCompressLevel //= -1
                ,int maxPackLen //= FZ_MAX_MSG_SIZE   
                ,bool removeOldMeta //= false
                ,const string& localAgentName //= "MsgAgent.SendAgent.SagentServantObj"
                ,const string& backupAgentName //= "MsgAgent.BackupAgent.SagentServantObj"
                );
    /**
     * 统计线程，每STAT_INTERVAL醒来一次，汇总上报监控，审计数据
     *
     */    
	void runStat();
	void startStatThread();	
    void pushLogs(const string & app
                        ,const string & server
                        ,const string & file
                        ,int64_t timeIn //ms
                        ,const vector<string> & vecLogs);

    /**
     * 获取logger。有读写锁。logger永生。
     *
     * @return MiddleLogger
     */
    MiddleLoggerPtrT getLogger(const MsgPackPtrT & packPtr);
    /**
     *  生成ASF标准名
     *
     */    
	string makeAsfKey(const string & app,const string & server,const string & file);
    /**
     * 将历史ASF中的异常字符标准化
     *
     */	
	string formatASF(const string & str);
    MsgPackPtrT makeMsgPack(const string & app
                                                        ,const string & server
                                                        ,const string & file
                                                        ,int64_t timeIn //ms
                                                        ,const std::vector<std::string> & vecLogs);
    void sendStatLog(vector<string> & vecStatLogV2);
    void enqueueResend(StatLogInfoPtrT & infoPtr);
    void sendStatLogInner(vector<string> & vecStatLog);
    void resendStatLog();
    void proxySend(StatLogInfoPtrT & infoPtr);

    int64_t getPackLogTime(int64_t logtime);
    int64_t getSendMs();
    void initKafkaProxy(const string & agentObj);
    void initStatLogProxy();
    void initFZConfigManagerValue(long startTime);
    void initGroupMetaFile( const string & metaFilePath
                            ,int maxLogsNum
                            , bool clearStart
                            ,size_t & dataFileSize
                            ,string & groupDataFileName);
    void initIpInfo();
    void initPropertyReport();
    void runStatOther();

private:
    //配置参数列表
    int _defaultLogsNum;
    string _metaFilePath;
    int _logThreads;
    bool _isTaflogMode;
    int _maxGroupNum ;
    int _localAgentPort ;
    string _agentObj;
    string _backupAgentObj;
    bool _useGzCompress;
    int _gzLevel;
    //port 
    int _instId;
    string _groupDataFileName;
    int _packQueueLen;
    int _maxPackSize;    
    size_t _dataFileSize;
    long _startTimeCost;
    bool _clearStart;
    
    //保护asfLoggers，读写锁，或许可以优化
    taf::TC_ThreadRWLocker _rwl;
    //asf 处理对象的map
    map<string, MiddleLoggerPtrT>    _asfLoggers;    

    //日志消费线程组
    LogConsumerThreadGroup _consumerThreadGroup;
    //统计线程
	TC_ThreadPool   _statPool;
	taf::TC_ThreadQueue<int> _statPoolTimerQueue;

    //供创建logger使用
    int _clientIp;
  	string _localIp;

    bool _terminated;
    bool _badMsgInfo;
    LogPrx _logPrx;             //转发代理

    taf::LogInfo _statLogInfoV2;
    taf::TC_ThreadQueue<StatLogInfoPtrT> _statLogResendQueue;

    TC_Atomic sendStatLogsNum;
    TC_Atomic retriedStatLogsNum;    
    TC_Atomic dropedStatLogsNum;    
    TC_Atomic packsOfStatLogsNum;        

};


}

#endif

