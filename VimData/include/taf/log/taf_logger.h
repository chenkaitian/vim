#ifndef __TAF_LOGGER_H__
#define __TAF_LOGGER_H__

#include "util/tc_logger.h"
#include "util/tc_file.h"
#include "util/tc_singleton.h"
#include "servant/Global.h"
#include "log/Log.h"
#include "servant/PropertyReport.h"

#define DYEING_DIR "taf_dyeing"
#define DYEING_FILE "dyeing"

namespace taf
{

/**
 * LOG的库说明
 * 用例请看test/log的代码
 * 1 循环日志采用LOG->error(), LOG->debug()
 * 2 循环日志不上传到服务器
 * 3 按天日志采用DLOG, FDLOG来记录
 * 4 按天日志也可以不上传到远程服务器:DLOG("")->disableRemote();
 * 5 按天日志可以改变每天一个文件的方式:
 *   DLOG("abc3")->setFormat("%Y%m%d%H");
 *   每个小时一个文件
 */

/*****************************************************************************
实现方式说明(只介绍按时间的日志, 会写到taflog):
    1 自定义时间日志的WriteT类:RemoteTimeWriteT
    2 在RemoteTimeWriteT类中, 写入到远程
    3 定义远程日志类:typedef TC_Logger<RemoteTimeWriteT, TC_RollByTime> RemoteTimeLogger;
    4 为了保证远程的写日志也是在单独线程处理,重新定义本地按天日志类
    5 自定义时间日志的WriteT类:TimeWriteT
    6 在TimeWriteT类中包含RemoteTimeLogger对象
    7 在TimeWriteT类的写入操作中, 写入本地文件后, 同时写入到RemoteTimeLogger对象中
    8 RemoteTimeLogger会在RemoteTimeWriteT对象中, 异步写入到远程
    9 从而本地文件写和远程写不在一个线程中.
*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////

class RollWriteT
{
public:
    RollWriteT();
    ~RollWriteT();

    void operator()(ostream &of, const deque<pair<int, string> > &ds);

    void setDyeingLogInfo(const string &sApp, const string &sServer, const string & sLogPath,
            int iMaxSize, int iMaxNum, const CommunicatorPtr &comm, const string & sLogObj);

protected:

    TC_RollLogger *_pDyeingRollLogger;

    static int  _iDyeingThread;

    string _sApp;
    string _sServer;
    string _sLogPath;
    int _iMaxSize;
    int _iMaxNum;

    /**
     * 染色远程滚动日志代理
     */
    LogPrx                _logPrx;


};


/**
 * 本地日志帮助类, 单件
 * 循环日志单件是永生不死的, 保证任何地方都可以使用
 * 当该对象析够以后, 则直接cout出来
 */
class TafRollLogger : public TC_Singleton<TafRollLogger, CreateUsingNew, PhoneixLifetime>
{
public:
		enum
		{
			NONE_LOG    = 1,	/**所有的log都不写*/
			ERROR_LOG   = 2,	/**写错误log*/
			WARN_LOG    = 3,	/**写错误,警告log*/
			DEBUG_LOG   = 4,	/**写错误,警告,调试log*/
			INFO_LOG    = 5		/**写错误,警告,调试,Info log*/
		};
public:
    typedef TC_Logger<RollWriteT, TC_RollBySize> RollLogger;

    /**
     * 设置本地信息
     * @param app, 业务名称
     * @param server, 服务名称
     * @param logpath, 日志路径
     * @param iMaxSize, 文件最大大小,字节
     * @param iMaxNum, 文件最大数
     */
    void setLogInfo(const string &sApp, const string &sServer, const string &sLogpath, int iMaxSize = 1024*1024*50, int iMaxNum = 10, const CommunicatorPtr &comm=NULL, const string &sLogObj="");

    /**
     * 设置同步写日志
     *
     * @param bSync
     */
    void sync(bool bSync = true);

    /**
     * 获取循环日志
     *
     * @return RollLogger
     */
    RollLogger *logger()          { return &_logger; }

    /**
     * 染色日志是否启用
     * @param bEnable
     */
    void enableDyeing(bool bEnable, const string& sDyeingKey = "");

protected:

    /**
     * 应用
     */
    string                  _sApp;

    /**
     * 服务名称
     */
    string                  _sServer;

    /**
     * 日志路径
     */
    string                  _sLogpath;

    /**
     * 循环日志
     */
    RollLogger           _logger;

    /**
     * 本地线程组
     */
    TC_LoggerThreadGroup    _local;

};

///////////////////////////////////////////////////////////////////////////////////////
//
/**
 * 写日志线程
 * 将写本地日志和远程分开到不同的线程
 * 作为单件存在, 且是永生不死的单件
 */
class TafLoggerThread : public TC_Singleton<TafLoggerThread, CreateUsingNew, PhoneixLifetime>
{
public:
    /**
     * 构造函数
     */
    TafLoggerThread();

    /**
     * 析够函数
     */
    ~TafLoggerThread();

    /**
     * 本地写日志线程
     */
    TC_LoggerThreadGroup* local();

    /**
     * 远程写日志线程
     *
     * @return TC_LoggerThreadGroup*
     */
    TC_LoggerThreadGroup* remote();

protected:

    /**
     * 本地线程组
     */
    TC_LoggerThreadGroup    _local;

    /**
     * 远程写线程组
     */
    TC_LoggerThreadGroup    _remote;
};

///////////////////////////////////////////////////////////////////////////////////////
class TimeWriteT;

/**
 * 远程的Log写操作类
 */
class RemoteTimeWriteT
{
public:
	RemoteTimeWriteT();
    ~RemoteTimeWriteT();

    /**
     * 构造函数
     */
    void setTimeWriteT(TimeWriteT *pTimeWrite);

    /**
     * 具体调用
     * @param of
     * @param buffer
     */
    void operator()(ostream &of, const deque<pair<int, string> > &buffer);

protected:
    /**
     * 同步到远程
     */
    void sync2remote(const vector<string> &buffer);

    /**
     * 染色日志同步到远程
     */
    void sync2remoteDyeing(const vector<string> &buffer);

protected:
    /**
     * 指针
     */
    TimeWriteT          *_pTimeWrite;

};

////////////////////////////////////////////////////////////////////////////
/**
 * 写Logger
 */
class TimeWriteT
{
public:
    typedef TC_Logger<RemoteTimeWriteT, TC_RollByTime> RemoteTimeLogger;

    typedef TC_Logger<TC_DefaultWriteT, TC_RollByTime> DyeingTimeLogger;

    /**
     * 构造
     */
    TimeWriteT();

    /**
     * 析够
     */
    ~TimeWriteT();

    /**
     * 设置基本信息
     * @param app, 应用名称
     * @param server, 服务名称
     * @param file, 日志文件名
     * @param sFormat, 格式
     * @param setdivision,set名称
     * @param sLogType,日志记录类型
     */
	void setLogInfo(const LogPrx &logPrx, const string &sApp, const string &sServer, const string &sFile, const string &sLogpath, const string &sFormat, const string& setdivision = "", const string& sLogType = "", const PropertyReportPtr &reportSuccPtr = NULL, const PropertyReportPtr &reportFailPtr = NULL,
		const size_t &discardNum=500000, const size_t &tranNum=2000);

    /**
     * 设置代理
     * @param logPrx 代理信息
     */
    void setLogPrx(const LogPrx &logPrx);

    /**
     * 远程日志功能打开或关闭
     * @param bEnable
     */
    void enableRemote(bool bEnable)         { _bRemote = bEnable; }

    /**
     * 本地日志功能功能打开或关闭
     * @param bEnable
     */
    void enableLocal(bool bEnable);

    /**
     * 染色日志功能打开或关闭
     * @param bEnable
     */
    void enableDyeing (bool bEnable, const string& sDyeingKey = "");

	/**
	*  打日志到方舟
	*/
	void enableFangZhou(bool bEnable);

	/**
	 * @brief 日志文件名是否带.log后缀
	 * @param bEnable
	 */
	void enableSufix(bool bEnable=true){_bHasSufix = bEnable;}
	/**
	 * @brief 是否允许框架在日志文件名上增加业务相关的标识
	 * @param bEnable
	 */
	void enablePrefix(bool bEnable=true){_bHasAppNamePrefix = bEnable;}
	/**
	 * @brief 日志文件名中用户自定义字符与日期字符间的连接符，默认是"_"
	 * @param str
	 */
	void setFileNameConcatStr(const string& str) {_sConcatStr = str;}

	/**
	 * @brief 框架中增加的日志内容之间的分割符，默认是"|"
	 * @param str
	 */
	void setSeparator(const string& str) {_sSepar = str;}

	/**
	 * @brief 框架中日期和时间之间是否需要加中括号[],有些统计由特殊需求；默认不加
	 * @param bEnable
	 */
	void enableSqareWrapper(bool bEnable) {_bHasSquareBracket = bEnable;}


    /**
     * 设置时间格式("%Y%m%d")
     * @param sFormat
     */
    void setFormat(const string &sFormat)   { _sFormat = sFormat;}

    /**
     * 具体调用
     * @param of
     * @param buffer
     */
    void operator()(ostream &of, const deque<pair<int, string> > &buffer);





protected:

    /**
     * 友元
     */
    friend class RemoteTimeWriteT;

    /**
     * 记录错误文件
     * @param buffer
     */
    void writeError(const vector<string> &buffer);

    /**
     * 记录错误文件
     * @param buffer
     */
    void writeError(const deque<pair<int, string> > &buffer);


   size_t getBufferSize();
   void activeFlush(deque<pair<int, string> >&buffer);


   void sync2fangzhou(vector<string> &v, time_t sec);
    /**
     * 初始化logger
     */
    void initError();

    /**
     * 初始化染色日志
     */
    void initDyeingLog();

	

protected:

    /**
     * 远程时间日志
     */
    RemoteTimeLogger    *_pRemoteTimeLogger;

    /**
     * 本地功能
     */
    bool                _bLocal;

    /**
     * 远程功能
     */
    bool                _bRemote;
    
   bool              _bFangZhou;

    /**
     * 远程服务句柄
     */
    LogPrx              _logPrx;

    /**
     * app名称
     */
    string              _sApp;

    /**
     * 服务名称
     */
    string              _sServer;

    /**
     * 日志文件名称
     */
    string              _sFile;

    /**
     * 时间格式
     */
    string              _sFormat;

    /**
     * 具体文件
     */
    string              _sFilePath;

    /**
     * 错误文件
     */
    TC_DayLogger        _elogger;

    /**
     * 缺省写模式
     */
    TC_DefaultWriteT    _wt;

    /**
     * 染色日志
     */
    static int                _bDyeing;

    /**
     * 染色日志目录路径
     */
    string              _sDyeingFilePath;

    /**
     * 远程时间日志
     */
    DyeingTimeLogger    *_pDyeingTimeLogger;

    /**
     * set分组信息
     */
    string                  _sSetDivision;

	/**
	 * 日志文件名是否带.log后缀
	 */
	bool					_bHasSufix;
	/**
	 * 是否允许框架在日志文件名上增加业务相关的标识
	 */
	bool 					_bHasAppNamePrefix;

	/**
	 * 日志文件名中用户自定义字符与日期字符间的连接符，默认是"_"
	 */
	string 					_sConcatStr;
	/**
	 * 分隔符
	 */
	 string        _sSepar;
	/**
	 * 日期部分是否加上[]
	 */
	 bool 		  _bHasSquareBracket;

	 /*
	  * 本地日志的记录类型,格式为TafLogType.toString()返回值,如果不采用TafLogType，则该值为""
	  */
	 string       _sLogType;
	 
	 /*
	 * 对于远程日志，上报同步到logser的成功量，默认不上报
	 */
	 PropertyReportPtr _reportSuccPtr;

	 /*
	 * 对于远程日志，上报同步到logser的失败量，默认不上报
	 */
	 PropertyReportPtr _reportFailPtr;

     // /*
     // * 对于方舟日志，单条大于3M，默认不上报
     // */
     // PropertyReportPtr _fzreportFailPtr;
	 /*
	 * 设置的内存最大存储条数 在模板中配置
	 */
	 size_t		_discardNum;

	 /*
	 *	设置转发条数 在模板中配置
	 */
	 size_t		_tranNum;



	 std::stringstream _stringStream;

	 vector<string> _streamBuffer;
	 std::string     _result;

};

////////////////////////////////////////////////////////////////////////////
/**
 * 远程日志帮助类, 单件
 */
class TafTimeLogger : public TC_HandleBase
                    , public TC_ThreadLock
                    , public TC_Singleton<TafTimeLogger, CreateUsingNew, DefaultLifetime>
{
public:

    //定义按时间滚动的日志
    typedef TC_Logger<TimeWriteT, TC_RollByTime> TimeLogger;

    /**
     * 构造
     */
    TafTimeLogger();

    /**
     * 析够
     */
    ~TafTimeLogger();

    /**
     * 设置本地信息
     * @param comm, 通信器
     * @param obj, 日志对象名称
     * @param app, 业务名称
     * @param server, 服务名称
     * @param logpath, 日志路径
     */
	void setLogInfo(const CommunicatorPtr &comm, const string &obj, const string &sApp, const string &sServer, const string &sLogpath, const string& setdivision = "", const bool &bLogStatReport = false, const size_t &discardNum = 500000, const size_t &tranNum = 2000);

	/**
     * 初始化设置时间格式("%Y%m%d")
     * 不要动态修改, 线程不安全
     * 如果有需要, 初始化后直接修改
     * @param sFormat, 文件名称, 为空表示缺省的时间日志
     */
	void initFormat(const string &sFile, const string &sFormat,const TafLogTypePtr& logTypePtr=NULL);
	void initFormat(const string &sApp, const string &sServer,const string &sFile, const string &sFormat,const TafLogTypePtr& logTypePtr=NULL);
    /**
     * 初始化设置时间格式("%Y%m%d")
     * 不要动态修改, 线程不安全
     * 如果有需要, 初始化后直接修改
     * @param sFormat, 文件名称, 为空表示缺省的时间日志
     * @param frequency 支持每多少天/小时/分钟，详见TC_logger.h中关于TafLogByDay,TafLogByHour,TafLogByMinute的描述
     *
     * 用法: 按两个小时记录日志
     * initFormat<TafLogByHour>("logfile",TafLogByHour::FORMAT,2);
     */
    template<class TafLogType>
    void initFormatWithType(const string &sFile, const string &sFormat,size_t frequency)
    {
        TafLogTypePtr logTypePtr = new TafLogType(sFormat,frequency);
        initFormat(sFile,sFormat,logTypePtr);
    }

    template<class TafLogType>
    void initFormatWithType(const string &sApp, const string &sServer,const string &sFile, const string &sFormat,size_t frequency)
    {
        TafLogTypePtr logTypePtr = new TafLogType(sFormat,frequency);
        initFormat(sApp,sServer,sFile,sFormat,logTypePtr);
    }
    /**
     * 获取时间日志
     * @param file
     */
    TimeLogger *logger(const string &sFile = "");

    /**
     * 获取时间日志
     * @param app, 业务名称
     * @param server, 服务名称
     * @param file
     */
    TimeLogger *logger(const string &sApp, const string &sServer,const string &sFile = "");

    /**
     * 同步写本地时间日志(远程日志一定是异步写的, 无法调整)
     * @param bSync
     */
    void sync(const string &sFile, bool bSync);

    /**
     * 远程时间日志
     * @param sFile, 文件名称, 为空表示缺省的时间日志
     * @param bEnable
     */
    void enableRemote(const string &sFile, bool bEnable);

    /**
     * 远程时间日志
     * @param sApp,应用名称
     * @param sServer,服务名称
     * @param sFile, 文件名称, 为空表示缺省的时间日志
     * @param bEnable
     */
	void enableRemoteEx(const string &sApp, const string &sServer,const string &sFile, bool bEnable);
    /**
     * 本地时间日志
     * @param sFile,文件名称, 为空表示缺省的时间日志
     * @param bEnable
     */
    void enableLocal(const string &sFile, bool bEnable);
    /**
     * 本地时间日志
     * @param sApp,应用名称
     * @param sServer,服务名称
     * @param sFile, 文件名称, 为空表示缺省的时间日志
     * @param bEnable
     */
	void enableLocalEx(const string &sApp, const string &sServer,const string &sFile, bool bEnable);
	/**
	* 打开方舟
	* @param 
	* @param bEnable
	*/
	void enableFangZhou(const string &sFile, bool bEnable);
	/**
	* ????????
	* @param sApp,应用名称
	* @param sServer,服务名
	* @param sFile, 文件名
	* @param bEnable
	*/
	void enableFangZhouEx(const string &sApp, const string &sServer, const string &sFile, bool bEnable);
	void enableSufix(bool bEnable=true){_bHasSufix = bEnable;}
	/**
	 * @brief 是否允许框架在日志文件名上增加业务相关的标识,影响全部日志文件
	 * @param bEnable
	 */
	void enablePrefix(bool bEnable=true){_bHasAppNamePrefix = bEnable;}
	/**
	 * @brief 日志文件名中用户自定义字符与日期字符间的连接符，默认是"_",影响全部日志文件
	 * @param str
	 */
	void setFileNameConcatStr(const string& str) {_sConcatStr = str;}

	/**
	 * @brief 框架中增加的日志内容之间的分割符，默认是"|",影响全部日志文件
	 * @param str
	 */
	void setSeparator(const string& str) {_sSepar = str;}

	/**
	 * @brief 框架中日期和时间之间是否需要加中括号[],有些统计由特殊需求；默认不加,影响全部日志文件
	 * @param bEnable
	 */
	void enableSqareWrapper(bool bEnable) {_bHasSquareBracket = bEnable;}
    /**
	 * @brief 是否输出本地日志文件，影响全部日志文件
	 * @param bEnable
     */
    void enableLocalLog(bool bEnable) {_bLocal = bEnable;}
	/**
	 * @brief 是否输出远程日志文件,影响全部日志文件
	 * @param bEnable
	 */
	void enableRemoteLog(bool bEnable) {_bRemote = bEnable;}

	void enableIp(int hasIp);

	void enableTime(int hasTime);

protected:

    /**
     * 初始化时间日志
     * @param pTimeLogger
     * @param sFile
     * @param sFormat
     * @param frequence, 每多少天/小时/分钟,单位是秒
     */
    void initTimeLogger(TimeLogger *pTimeLogger, const string &sFile, const string &sFormat,const TafLogTypePtr& logTypePtr=NULL);

    /**
     * 初始化时间日志
     * @param pTimeLogger
     * @param sApp
     * @param sServer
     * @param sFile
     * @param sFormat
     * @param frequence, 每多少天/小时/分钟,单位是秒
     */

    void initTimeLogger(TimeLogger *pTimeLogger,const string &sApp, const string &sServer, const string &sFile, const string &sFormat,const TafLogTypePtr& logTypePtr=NULL);

protected:

    /**
     * 通信器
     */
    CommunicatorPtr         _comm;

    /**
     * 远程服务句柄
     */
    LogPrx                  _logPrx;

    /**
     * 应用
     */
    string                  _sApp;

    /**
     * 服务名称
     */
    string                  _sServer;

    /**
     * 日志路径
     */
    string                  _sLogpath;

    /**
     * 缺省按天日志
     */
    TimeLogger              *_pDefaultLogger;

    /**
     * 远程日志
     */
    map<string, TimeLogger*>    _loggers;
    /**
     * set分组信息
     */
    string                  _sSetDivision;

	/**
	 * 是否带.log后缀
	 */
	bool					_bHasSufix;
	/**
	 * 是否允许框架在日志文件名上增加业务相关的标识
	 */
	bool 					_bHasAppNamePrefix;

	/**
	 * 日志文件名中用户自定义字符与日期字符间的连接符，默认是"_"
	 */
	string 					_sConcatStr;
	/**
	 * 分隔符
	 */
	string        _sSepar;
	/**
	 * 日期部分是否加上[]
	 */
	bool 		  _bHasSquareBracket;
    /**
     * 是否输出本地日志
     */
	bool         _bLocal;
    /**
     * 是否输出远程日志
     */
	bool        _bRemote;
    /**
     * 是否打印到方舟
     */

    bool         _bFangZhou;

	/*
	* 服务日志上报logser是否上报成功数量
	*/
	bool		_bLogStatReport;

	/*
	 * 设置的内存最大存储条数 在模板中配置
	 */
	size_t		_discardNum;
	
	/*
	* 设置转发条数 在模板中配置
	*/
	size_t		_tranNum;

	time_t      _tt;

	
	int        _hasTime;
	/*????ip*/
	int        _hasIp;


};

/**
 * 染色开关类，析构时关闭
 */
class TafDyeingSwitch
{
public:
    /**
     * 构造函数，默认不打开染色日志
     */
    TafDyeingSwitch()
    :_bNeedDyeing(false)
    {
    }

    /**
     * 析构函数，关闭已打开的染色日志
     */
    ~TafDyeingSwitch()
	{
		if(_bNeedDyeing)
		{
			TafRollLogger::getInstance()->enableDyeing(false);

			ServantProxyThreadData * td = ServantProxyThreadData::getData();
            assert(NULL != td);
			if (td)
			{
	            td->_bDyeing = false;
				td->_dyeingKey = "";
			}
		}
	}

	/**
	 * 获取染色的key
	 *
	 * @author coonzhang (2011/8/19)
	 * @param key
	 * @return bool
	 */
	static bool getDyeingKey(string & sDyeingkey)
	{
		ServantProxyThreadData * td = ServantProxyThreadData::getData();
        assert(NULL != td);

		if (td && td->_bDyeing == true)
		{
			sDyeingkey = td->_dyeingKey;
			return true;
		}
		return false;
	}

    /**
     * 启用染色日志
     */
    void enableDyeing(const string & sDyeingKey = "")
	{
		TafRollLogger::getInstance()->enableDyeing(true);

		ServantProxyThreadData * td = ServantProxyThreadData::getData();
        assert(NULL != td);
		if(td)
		{
	        td->_bDyeing	= true;
			td->_dyeingKey = sDyeingKey;
		}
		_bNeedDyeing = true;
        _sDyeingKey = sDyeingKey;
	}

protected:
    bool _bNeedDyeing;
    string _sDyeingKey;
};

/**
 * 循环日志
 */
#define LOG             (TafRollLogger::getInstance()->logger())

/**
 * @brief 按级别循环日志宏
 *
 * @param level 日志等级,TafRollLogger::INFO_LOG,TafRollLogger::DEBUG_LOG,TafRollLogger::WARN_LOG,TafRollLogger::ERROR_LOG
 * @msg 日志内容语句,包括<<重定向符连接的语句,如 "Demo begin" << " testing !" <<endl;
 *
 * @用法:
 *       标准输出流方式: cout << "I have " << vApple.size() << " apples!"<<endl;
 *       框架宏方式:     LOGMSG(TafRollLogger::INFO_LOG,"I have " << vApple.size() << " apples!"<<endl);
 */
#define LOGMSG(level,msg...) do{ if(LOG->IsNeedLog(level)) LOG->log(level)<<msg;}while(0)

/**
 * @brief 按级别循环日志
 *
 * @msg 日志内容语句,包括<<重定向符连接的语句,如 "Demo begin" << " testing !" <<endl;
 *
 * @用法:
 *       标准输出流方式: cout << "I have " << vApple.size() << " apples!"<<endl;
 *       框架宏方式:     TLOGINFO("I have " << vApple.size() << " apples!"<<endl);
 */
#define TLOGINFO(msg...)    LOGMSG(TafRollLogger::INFO_LOG,msg)
#define TLOGDEBUG(msg...)   LOGMSG(TafRollLogger::DEBUG_LOG,msg)
#define TLOGWARN(msg...)    LOGMSG(TafRollLogger::WARN_LOG,msg)
#define TLOGERROR(msg...)   LOGMSG(TafRollLogger::ERROR_LOG,msg)

/**
 * 按天日志
 */
#define DLOG            (TafTimeLogger::getInstance()->logger()->any())
#define FDLOG(x)        (TafTimeLogger::getInstance()->logger(x)->any())
#define FFDLOG(x,y,z)   (TafTimeLogger::getInstance()->logger(x,y,z)->any())

/**
 *  按天日志局部使能开关，针对单个日志文件进行使能，请在所有按天日志输出前调用
 */
#define TENREMOTE_FDLOG(swith,sApp,sServer,sFile) (TafTimeLogger::getInstance()->enableRemoteEx(sApp,sServer,sFile,swith))
#define TENLOCAL_FDLOG(swith,sApp,sServer,sFile) (TafTimeLogger::getInstance()->enableLocalEx(sApp,sServer,sFile,swith))

/**
 * 按天日志全局使能开关，请在所有按天日志输出前调用
 */
#define TENREMOTE(swith) (TafTimeLogger::getInstance()->enableRemoteLog(swith))
#define TENLOCAL(swith)  (TafTimeLogger::getInstance()->enableLocalLog(swith))
}

#endif

