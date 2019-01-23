#ifndef _AsyncLogThread_H_
#define _AsyncLogThread_H_
#include "util/tc_singleton.h"
#include "util/tc_thread.h"
#include "util/tc_monitor.h"
#include "util/tc_file.h"
#include "util/tc_timeprovider.h"
#include "util/tc_common.h"
#include "log/Log.h"

#define SEND_NUM 1000
#define MAX_SEND_SIZE 5242880

namespace taf
{
	/*
	* 异步转发打远程失败的日志，不进行排重处理
	*/

	class AsyncLogThread :public TC_Thread, public TC_ThreadLock, public TC_Singleton<AsyncLogThread>
	{
	public:
		/**
		* 构造函数
		* @param Log
		*/
		AsyncLogThread();

		/**
		* 析构函数
		* @param Log
		*/
		~AsyncLogThread();

		/*
		* 初始化函数，日志路径、代理等
		*/
		void init(const CommunicatorPtr &comm, const string &setdivision, const bool &bTran);

		/*
		* 开启处理线程
		*/
		void run();

		/*
		*结束处理线程
		*/
		void terminate();

		/*
		* 是否设置了异步转发
		*/

		const bool isAsyncTran() const
		{
			return _bTran;
		}

	private:

		/*
		* 获取需要转发的日志名
		*/
		void getTranLogName(set<string> &vFiles);

		/*
		* 逐一转发落地日志
		*/
		void dealLog(set<string> &vFiles);

		/*
		* 发送失败日志
		*/
		void sendLog(const string sFileName, const int &iOffset);

		/*
		* 获取a.s.f中的filename与日志格式
		*/
		void getRealFileName(const string &sOriginalFile, string &sFile, string &sFormat);

		/*
		* 从日志文件中获取需要转发的日志，小于5M或1000条
		*/
		void getLogFromFile(const string &sFileName, const int &iOffSet, int &iReadSize, vector<string> &vLogs);

	private:
		bool _bTerminate;

		bool _bTran;				//模板是否配置落地转发，析构使用

		CommunicatorPtr _comm;
		LogPrx _logPrx;				//转发代理
		string _logPath;			//日志真实路径
		string _logInfoFile;		//缓存转发位置的文件

		LogInfo _stBaseInfo;		//通用logInfo
	};
}

#endif
