#ifndef GROUP_MANAGER_H_
#define GROUP_MANAGER_H_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <set>
#include <map>
#include <exception>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <util/tc_monitor.h>
#include <jmem/jmem_hashmap.h>
#include "MsgHelper.h"
#include "util/tc_atomic.h"
#include "servant/Application.h"
#include <servant/Communicator.h>
#include <servant/CommunicatorFactory.h>

#include "jmem/jmem_hashmap_malloc.h"


using std::string;
using std::map;
using Msg::GroupInfoKey;
using Msg::GroupInfo;

namespace mid
{
/**
 * 记录请求的时间戳和序号，用于排重
 */

const int FZ_GM_GET_OK = 0;
const int FZ_GM_GET_FAILED = -1;
const int FZ_GM_GET_EXPAND = -2;

const int FZ_GM_CHECK_OK = 2;
const int FZ_GM_CHECK_ERROR = 3;
const int FZ_GM_PARSE_OK = 100;
const int FZ_GM_PARSE_ERROR = 101;

const int FZ_GM_EXPAND_OK = 200;
const int FZ_GM_EXPAND_ERROR = 201;
const int FZ_GM_EXPAND_MAX = 202;


#define FZ_GM_VALUE_SEP "|"
#define FZ_GM_KEY_SEP "_"


const size_t FZ_GM_FILE_EXPAND_SPAN_SIZE = 4*1024*1024;
const size_t FZ_GM_FILE_MAX_EXPAND_SIZE = 16*1024*1024;

const unsigned long long FZ_GM_FILE_MAX_SIZE = 2*1024*1024*1024L;

const double FZ_GM_EXPAND_RATIO = 0.8;


class FZDataMapPtr:public TC_HandleBase
{
public:

    taf::JceHashMapMalloc<taf::ThreadLockPolicy, taf::FileStorePolicy> _dataMap;
};

typedef taf::TC_AutoPtr<FZDataMapPtr> FZDataMapPtrT;

 
class FZGroupManager:
    public taf::TC_Singleton<FZGroupManager, CreateUsingNew, DefaultLifetime>
{
public:

    void init(const std::string groupDataFile, size_t iSize, bool cleanStart);
    bool needExpand();
    int expand();
    void reinitMetaFile(const std::string groupDataFile, size_t iSize);
    unsigned long getCurFileSize(const std::string & gf);
    void initMetaFile(const std::string groupDataFile, size_t iSize) ;
    size_t getExpandSize();

    int32_t dataMapSet(const GroupInfoKey& groupKey, const GroupInfo & groupValue);

    int32_t dataMapGet(const GroupInfoKey& groupKey, GroupInfo & groupValue);
    int32_t parseValue(const string& value , GroupInfo & groupValue);

    //检查一个文件的group信息是否都存在
    int initAsfSeq(const string& loggerKeyMd5, int32_t maxPartition);
    int initAsfSeqInner(const string& loggerKeyMd5, int32_t maxPartition);

    //取序列化种子
    int getSeqAndInc(const GroupInfoKey& groupKey, int32_t &newSeq, int32_t &newGid, const int log_groups);

    int32_t getAndIncInner(const GroupInfoKey& groupKey, int32_t &newSeq, int32_t &newGid, const int log_groups) ;

    int64_t getSeedMs();

    void removeFile(const std::string & gf);

    int getRandStart(int gid,int randPos);

    FZGroupManager();
    ~FZGroupManager();
    size_t getFileSize();
    size_t getExpandTimes();
    size_t getSizeOfMap();

private:
    size_t dataMapSizeConf; 
    string metaFileName;
	taf::TC_Atomic _sizeOfMap;
	//taf::JceHashMap<GroupInfoKey, GroupInfo, taf::ThreadLockPolicy, taf::FileStorePolicy> _dataMap;
	//taf::JceHashMapMalloc<taf::ThreadLockPolicy, taf::FileStorePolicy> _dataMap;

    int expandTimes;
    size_t expandSize;    
	//保护dataMap
	TC_ThreadLock _mutexDataMap;
	FZDataMapPtrT dataMapPtr;
};

}

#endif /* SEQ_MANAGER_H_ */
