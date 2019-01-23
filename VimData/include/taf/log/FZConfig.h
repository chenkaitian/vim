/*
 * msg_conf.h
 *
 *  Created on: 2015.4.10
 *      Author: jouvechen
 */

#ifndef MSG_CONF_H_
#define MSG_CONF_H_
#include <iostream>
#include "log/taf_logger.h"
#include "util/tc_timeprovider.h"
#include "MsgWithID.h"
//#include "lib_snappy/snappy.h"
#include "zlib.h"

#include "util/tc_singleton.h"

using namespace std;
using namespace taf;



namespace mid
{

#define GET_CONF mid::FZConfigManager::getInstance()
const int FZ_LOCAL_GROUP_MAX_NUM = (1024);
const int FZ_LOCAL_GROUP_ALIVE_DEFAULT_NUM = (120);

const int FZ_MAX_MSG_SIZE = (3*1024*1024);
const int FZ_DEF_LOG_QUEUE_MAX = (1000);
#define DEF_GROUP_FILE "fz.groupSeq.dat"

#define __FZ_LOG_FLAG__  "FZ|"<<__FILE__<<":"<<__LINE__<<"|"<<__FUNCTION__

#define SEP "|"
#define SEP_PLUS +"|"+


#define FZLOG(x) TLOGINFO(  "[" << __FZ_LOG_FLAG__ << "]|" << x)
#define FZERROR(x) TLOGERROR( "[" << __FZ_LOG_FLAG__ << "]|" << x)
#define FZWARN(x) TLOGWARN(  "[" << __FZ_LOG_FLAG__ << "]|" << x)

#define FZGMLOG(x) TLOGERROR( "GM[" << __FZ_LOG_FLAG__ << "]|" << x)


/////////////////////////////////////////////////////
class FZConfigManager : public TC_Singleton<FZConfigManager>
{
public:
	FZConfigManager();
	~FZConfigManager();

	void setCleanMeta(bool cleanMeta);
	bool getCleanMeta();
    void setIsTafLogMode(bool taflogMode);
    string getIsTafLogMode();

    void setGZLevel(int gzLevel);
    int getGZLevel();

    void setDefaultLoggers(size_t ml);
    size_t getDefaultLoggers();
	
    void setStartTimeCost(int tc);
    int getStartTimeCost();

    void setCpuNum(int tc);
    int getCpuNum();

    void setFreeMem(int tc);
    int getFreeMem();

    void setThreadNum(int tc);
    int getThreadNum();
    

	void setInstID(string id);
	string getInstID();

	void setGroupDataFileSize(int gsfs);
	int getGroupDataFileSize();

	void setUseGZ(bool isGz);
	bool getUseGZ();

	void setLogQueueSize(int log_queue_size);
	int getLogQueueSize();

	void setMaxPackSize(taf::Int64 pack_size);
	int getMaxPackSize();

	void setGroupDataFileName(const string& fileName);
	string getGroupDataFileName();

	void setMaxGroupNums(int max_group_nums);
	int getMaxGroupNums();


private:
	string _groupDataFileName;
    int _gzCompLevel;
    size_t _defaultLoggers;    
	bool _useGz;
	bool _cleanMetaBeforeStart;		
	//日志队列大小
	int _logQueueSize;
    string _instId;
	taf::Int64 _maxPackSize;
	int _groupDataFileSize;	
	int _maxGroupNums;
    int _startMetaTimeCost;
    int _startTimeCost;    
    int _cpuNum;
    int _freeMem;
    int _threadNum;
    string _isTaflogMode;    
};

};
#endif /* MSG_CONF_H_ */
