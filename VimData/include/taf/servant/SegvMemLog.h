
#ifndef __SEGV_MEM_LOG_H
#define __SEGV_MEM_LOG_H

#include <stdio.h>
#include <time.h>
#include "util/tc_mmap.h"
#include "util/tc_monitor.h"
#include "util/tc_singleton.h"

/*
* 使用示例
* 1.在框架服务里面使用内存日志
*   调用如下：
*   vector<char> vRequest;
*   vRequest.push_back('a');
*   TAFMEMLOG0(TAF_LOG_INT, "key:", key, TAF_LOG_STRING, "caller_ip:", "172.0.0.1", TAF_LOG_BUF, "packet:", vRequest.data(), vRequest.size());
* 2.在纯客户端使用内存日志
*   调用如下：
*   TafSegvMemLog::getInstance()->InitBinLogMMapFile(sMemLogMMapFile.c_str(), sMemLogDumpFile.c_str(), iShiftType, iMaxLogNum, iMaxMemLogFileSize, iMaxLogRec);
*   TafSegvMemLog::getInstance()->setSwitch(true);
*   vector<char> vRequest;
*   vRequest.push_back('a');
*   TAFMEMLOG0(TAF_LOG_INT, "key:", key, TAF_LOG_STRING, "caller_ip:", "172.0.0.1", TAF_LOG_BUF, "packet:", vRequest.data(), vRequest.size());
* 3.关闭内存日志
*   调用如下：
*   TafSegvMemLog::getInstance()->setSwitch(false);
*/
namespace taf
{

#if defined __x86_64__

#define TAF_C2_INT32 int
#define TAF_C2_UINT64 unsigned long
#define TAF_C2_TIME int
#define TAF_C2_SIZE unsigned int

#define TAF_C2_PRIu32 "u"
#define TAF_C2_PRId32 "d"
#define TAF_C2_PRIx32 "x"
#define TAF_C2_SCNu32 "u"
#define TAF_C2_SCNd32 "d"
#define TAF_C2_PRIu64 "lu"

#else

#define TAF_C2_INT32 long
#define TAF_C2_UINT64 unsigned long long
#define TAF_C2_TIME time_t
#define TAF_C2_SIZE size_t

#define TAF_C2_PRIu32 "lu"
#define TAF_C2_PRId32 "ld"
#define TAF_C2_PRIx32 "lx"
#define TAF_C2_SCNu32 "lu"
#define TAF_C2_SCNd32 "ld"
#define TAF_C2_PRIu64 "llu"

#endif

#define TAF_LOG_TAIL 		(unsigned char)0 //标志变参的结束
#define TAF_LOG_CHAR    	(unsigned char)1
#define TAF_LOG_SHORT    	(unsigned char)2
#define TAF_LOG_INT     	(unsigned char)3
#define TAF_LOG_LONG    	(unsigned char)4
#define TAF_LOG_DOUBLE  	(unsigned char)5
#define TAF_LOG_STRING  	(unsigned char)6 
#define TAF_LOG_DESC_STRING (unsigned char)7 
#define TAF_LOG_NSTRING  	(unsigned char)8
#define TAF_LOG_BUF			(unsigned char)9
#define TAF_LOG_BUF2		(unsigned char)10
#define TAF_LOG_ULONG    	(unsigned char)11
//
#define TAF_LOG_TIME0		(unsigned char)65
#define TAF_LOG_TIME1		(unsigned char)66
#define TAF_LOG_TIME2		(unsigned char)67

#define TAF_LOG_BASIC_ARGS 	TAF_LOG_STRING, "Function: ", __FUNCTION__, \
							TAF_LOG_INT, "Line: ", __LINE__ \

class TafMemLogInterface : public TC_ThreadLock
{
public:
	static const unsigned int	TAF_MMAP_LOG_BUF_LEN		= 16000000;
	static const int			TAF_MAX_MMAP_LOG_REC_LEN	= 65535;

	struct TafMemLogFile
	{
		FILE		*pLogFile;
		char		sBaseFileName[128];
		char		sLogFileName[128];
		int			iShiftType;			// 0 -> shift by size,  1 -> shift by LogCount, 2 -> shift by interval, 3 ->shift by day, 4 -> shift by hour, 5 -> shift by min
		int			iMaxLogNum;
		TAF_C2_INT32	lMaxSize;
		TAF_C2_INT32	lMaxCount;
		TAF_C2_INT32	lLogCount;
		TAF_C2_TIME		lLastShiftTime;
	};

	struct TafMemLogMMap
	{
		unsigned	TAF_C2_INT32 lHeadPos;
		unsigned	TAF_C2_INT32 lTailPos;	//下一个可用的位置
		char		sReserved[20];
		char		sLogBuf[TAF_MMAP_LOG_BUF_LEN]; 	
	};

	struct TafMemLogMMapFile
	{
		TafMemLogFile		stLogFile;
		TafMemLogMMap		*pstLogMMap;
		TAF_C2_INT32		lMaxLogRec;			//最多Dump的最近记录数
		char				sReserved[20];
	};
public:

	TafMemLogInterface()
	: _bFileMMapFlag(true)
	{}

	~TafMemLogInterface() {}

	/* 
	 *lMaxLogNum 最多文件数目
	 *lMAX	每个文件最大长度
	 *lMaxLogRec Dump内存到文件时最多dump的记录数,0表示不限制
	 */
	int InitBinLogMMapFile(const char* sMMapFile, const char* sLogBaseName, TAF_C2_INT32 lShiftType, TAF_C2_INT32 lMaxLogNum, TAF_C2_INT32 lMAX, TAF_C2_INT32 lMaxLogRec, bool bFlag);

	/***
	  SlogBin(1,
  			//类型, "注释", log的内容, [内容的长度,]
  			TAF_LOG_CHAR, "cmd type:", 5,
  			TAF_LOG_INT, "BufLen type:", 530,
  			TAF_LOG_STRING, "Msg:", "Hello world\n",
  			TAF_LOG_NSTRING, "Msg2:", sMsg, strlen(sMsg),
  			TAF_LOG_BUF, "Pkg:", &stRpkg, sizeof(stRpkg),
 			TAF_LOG_TAIL);
	 ***/
	int SLogBin(int iLogTime, ...);

	void setFileMMapFile(bool bFlag) { _bFileMMapFlag = bFlag; }

private:

	//////////////////////////////////////////////////
	int ShiftFiles(TafMemLogFile* pstLogFile);

	int GetBinLogMMap(TafMemLogMMap **pstLogMMap, const char* sMMapFile);

	int DumpBinLogMMapToFile(TafMemLogMMapFile* pstLogMMapFile, int iLogTime, const char* sFormat, ...);

	int ParseMMap(TafMemLogFile* pstLogFile, char *sTmpShm, int iHeadRec, int iTailRec);

	int ParseRecord(TafMemLogFile* pstLogFile, char *sBuf, int iRecordLen);

	//////////////////////////////////////////////////
	const char *DumpPackage(void *pPkg, int iPkgLen);

	const char *DumpMemory(void *pMem, TAF_C2_SIZE uDumpOffset, TAF_C2_SIZE uDumpLen);

	const char *DumpHex(void *pMem, TAF_C2_SIZE uDumpOffset, TAF_C2_SIZE uDumpLen);

	char *DateTimeStrRaw(const time_t tTime);

	char *CurrDateTimeStr();

	char *DateTimeStr(const TAF_C2_TIME *mytime);

	//////////////////////////////////////////////////
	int AddDWord(char **p, int *piLenLeft, unsigned TAF_C2_INT32 lValue);

	int AddHChar(char **p, int *piLenLeft, char cValue);//

	int AddHDWord(char **p, int *piLenLeft, unsigned TAF_C2_INT32 lValue);//

	int AddHString(char **p, int *piLenLeft, const char *pBuf, unsigned short shBufLen);//

	int AddHWord(char **p, int *piLenLeft, unsigned short shValue);//

	int AddHDWord64(char **p, int *piLenLeft, TAF_C2_UINT64 lValue);

	int GetDWord(char **p, int *piLen, unsigned TAF_C2_INT32 *plValue);

	int GetHBuffer(char **p, int *piLen, char *pBuf, int *piBufLen);//

	int GetHChar(char **p, int *piLen, char *cValue);//

	int GetHDWord(char **p, int *piLen, unsigned TAF_C2_INT32 *plValue);//

	int GetHWord(char **p, int *piLen, unsigned short *pshValue);//

	int GetHDWord(char **p, int *piLen, TAF_C2_UINT64 *plValue);

private:
	bool				_bFileMMapFlag;//mmap是否成功映射到文件
	TC_Mmap             _mmap;
	TafMemLogMMapFile	_memLog;

};

///////////////////////////////////////////////////////////////////////////////////
class TafSegvMemLog : public TC_Singleton<TafSegvMemLog, CreateUsingNew, DefaultLifetime>
{
public:

	TafSegvMemLog() : _bOff(false) {}

	~TafSegvMemLog() {}

	void setSwitch(bool bFlag);// { _bOff = bFlag; }

	bool getSwitch() { return _bOff; }

	int InitBinLogMMapFile(const char* sMMapFile, const char* sLogBaseName, TAF_C2_INT32 lShiftType, TAF_C2_INT32 lMaxLogNum, TAF_C2_INT32 lMAX, TAF_C2_INT32 lMaxLogRec, bool bFlag);

	TafMemLogInterface * getImp() { return &_memLogImp; }

	void setFileMMapFlag(bool bFlag) { _memLogImp.setFileMMapFile(bFlag); }

private:
	bool				_bOff;
	TafMemLogInterface	_memLogImp;
};

#define TAFMEMLOG0(...) do{ if(TafSegvMemLog::getInstance()->getSwitch()) TafSegvMemLog::getInstance()->getImp()->SLogBin(0, TAF_LOG_BASIC_ARGS, __VA_ARGS__, TAF_LOG_TAIL); }while(0)

#define TAFMEMLOG1(...) do{ if(TafSegvMemLog::getInstance()->getSwitch()) TafSegvMemLog::getInstance()->getImp()->SLogBin(1, TAF_LOG_BASIC_ARGS, __VA_ARGS__, TAF_LOG_TAIL); }while(0)

#define TAFMEMLOG2(...) do{ if(TafSegvMemLog::getInstance()->getSwitch()) TafSegvMemLog::getInstance()->getImp()->SLogBin(2, TAF_LOG_BASIC_ARGS, __VA_ARGS__, TAF_LOG_TAIL); }while(0)

}

#endif
