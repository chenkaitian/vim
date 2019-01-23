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
	* �첽ת����Զ��ʧ�ܵ���־�����������ش���
	*/

	class AsyncLogThread :public TC_Thread, public TC_ThreadLock, public TC_Singleton<AsyncLogThread>
	{
	public:
		/**
		* ���캯��
		* @param Log
		*/
		AsyncLogThread();

		/**
		* ��������
		* @param Log
		*/
		~AsyncLogThread();

		/*
		* ��ʼ����������־·���������
		*/
		void init(const CommunicatorPtr &comm, const string &setdivision, const bool &bTran);

		/*
		* ���������߳�
		*/
		void run();

		/*
		*���������߳�
		*/
		void terminate();

		/*
		* �Ƿ��������첽ת��
		*/

		const bool isAsyncTran() const
		{
			return _bTran;
		}

	private:

		/*
		* ��ȡ��Ҫת������־��
		*/
		void getTranLogName(set<string> &vFiles);

		/*
		* ��һת�������־
		*/
		void dealLog(set<string> &vFiles);

		/*
		* ����ʧ����־
		*/
		void sendLog(const string sFileName, const int &iOffset);

		/*
		* ��ȡa.s.f�е�filename����־��ʽ
		*/
		void getRealFileName(const string &sOriginalFile, string &sFile, string &sFormat);

		/*
		* ����־�ļ��л�ȡ��Ҫת������־��С��5M��1000��
		*/
		void getLogFromFile(const string &sFileName, const int &iOffSet, int &iReadSize, vector<string> &vLogs);

	private:
		bool _bTerminate;

		bool _bTran;				//ģ���Ƿ��������ת��������ʹ��

		CommunicatorPtr _comm;
		LogPrx _logPrx;				//ת������
		string _logPath;			//��־��ʵ·��
		string _logInfoFile;		//����ת��λ�õ��ļ�

		LogInfo _stBaseInfo;		//ͨ��logInfo
	};
}

#endif
