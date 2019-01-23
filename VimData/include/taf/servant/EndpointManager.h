#ifndef __TAF_ENDPOINT_MANAGER_H_
#define __TAF_ENDPOINT_MANAGER_H_

#include "util/tc_consistent_hash_new.h"
#include "servant/EndpointInfo.h"
#include "servant/EndpointF.h"
#include "servant/QueryF.h"
#include "servant/AppProtocol.h"

namespace taf
{

enum  GetEndpointType
{
    E_DEFAULT = 0,
    E_ALL = 1,
    E_SET = 2,
    E_STATION = 3
};

enum  EndpointWeightType
{
    E_LOOP = 0,
    E_STATIC_WEIGHT = 1,
    E_DYNAMIC_WEIGHT = 2,
};

class QueryEpBase : public QueryFPrxCallback
{
public:
	static string WEIGHT_SUCC_RATIO;  //成功率占比字符串常量

    static string WEIGHT_AVGTIME_RATIO;  //平均耗时占比字符串常量

    static string WEIGHT_CPULOAD_RATIO; //cpu负载占比字符串常量

	static string WEIGHT_ADJUST_THRESHOLD; //权重调节阀值

public:
    QueryEpBase(Communicator * pComm,bool bFirstNetThread,bool bThread);
	virtual ~QueryEpBase(){};
    void callback_findObjectById4All(taf::Int32 ret,
        const vector<taf::EndpointF>& activeEp,
        const vector<taf::EndpointF>& inactiveEp);

    void callback_findObjectById4All_exception(taf::Int32 ret);

    void callback_findObjectById4Any(taf::Int32 ret,
        const vector<taf::EndpointF>& activeEp,
        const vector<taf::EndpointF>& inactiveEp);

    void callback_findObjectById4Any_exception(taf::Int32 ret);

    void callback_findObjectByIdInSameGroup(taf::Int32 ret,
            const vector<taf::EndpointF>& activeEp,
            const vector<taf::EndpointF>& inactiveEp);

    void callback_findObjectByIdInSameGroup_exception(taf::Int32 ret);

    void callback_findObjectByIdInSameSet(taf::Int32 ret,
            const vector<taf::EndpointF>& activeEp,
            const vector<taf::EndpointF>& inactiveEp);

    void callback_findObjectByIdInSameSet_exception(taf::Int32 ret);

    void callback_findObjectByIdInSameStation(taf::Int32 ret,
            const vector<taf::EndpointF>& activeEp,
            const vector<taf::EndpointF>& inactiveEp);

    void callback_findObjectByIdInSameStation_exception(taf::Int32 ret);

	int setLocatorPrx(QueryFPrx prx);

	bool getDirectProxy() { return _bDirectProxy; }

public:

    virtual void notifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive,bool bSync) = 0;

    //从主控请求到数据了 最开始调用主控 要通知
    virtual void doNotify() = 0;

    //初始化接口 设置obj和主控名字
    bool init(const string & sObjName,const string & sLocator,const string& setName="");
protected:

    //刷新主控
    void refreshReg(GetEndpointType type,const string & sName);

private:
    void setEndpoints(const string & sEndpoints, set<EndpointInfo> & setEndpoints, set<string> & sEndpointInfo);

    //设置obj名字,如果是间接连接，则设置主控代理，并从缓存中加载相应的列表
    void setObjName(const string & sObjName);

    void doEndpoints(const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp,int iRet,bool bSync=false);
    void doEndpointsExp(int iRet,bool bSync = false);

    //主控信息刷新到缓存文件
    void setEndPointToCache(bool bInactive);

	void getWeightRatio(const map<std::string, std::string> & mRspContext);

protected:

    //是否直连标识
    bool _bDirectProxy;

    //表示是否接口的请求
    bool _bThread;

	//第一次使用cache获取ip列表
	bool _bFirstUseCache;

	//从主控拉取的节点信息与上次是否一样
	bool _bSameEndpoints;

	//采集到的cpu信息是否正常
	bool _bCpuLoadValid;

	//权重类型
	EndpointWeightType _weightType;

	//成功率占比
	size_t _iSuccRatio;

	//平均延时占比
	size_t _iAvgTimeRatio;

	//cpu负载占比
	size_t _iCpuLoadRatio;

	//权重调节阀值
	int	_iWeightAdjustThreshold;

    //obj name
    string _sObjName;

	//指定set调用的setid,默认为空，如果有值，则说明是指定set调用
	string _invokeSetId;

	QueryFPrx _queryFPrx;

    Communicator * _pCommunicator;

	//活跃节点列表信息，不含cpu负载等信息
    set<string> _sActiveEndpoints;

    //不活跃节点列表 在管理平台里面还是有的，不含cpu负载等信息
    set<string> _sInactiveEndpoints;

    //活跃节点列表
    set<EndpointInfo> _activeEndpoints;

    //不活跃节点列表 在管理平台里面还是有的
    set<EndpointInfo> _inactiveEndpoints;

    //数据是否有效。只有请求过主控或者从文件load过来的数据才是有效数据
    //初始化的时候是无效的数据
    bool _valid;


    //是否第一个网络线程
    bool _bFirstNetThread;

	//主控地址
	string _sLocator;
private:

    //请求列表的频率,单位秒
    int _iRefreshInterval;

    //请求失败的间隔时间
    int _iFailInterval;

    //请求回来活跃列表为空的间隔时间
    int _iActiveEmptyInterval;

    //连续请求失败的次数
    int _iMaxFailTimes;

    //达到连续请求失败的次数 以后的时间间隔
    int _iManyFailInterval;

    //请求主控的超时时间
    int _iTimeOut;

    //请求主控的等待时间
    int _iWaitTime;

    //请求的超时时间(绝对时间) 防止请求回调丢了。一直在正在请求状态
    int64_t _iRequestTimeout;

    //是否正在请求
    bool _bRequest;

    //连续失败的次数
    int _iFailTimes;

    //下次更新时间,单位秒
    int64_t _iRefreshTime;

    //int64_t _lastCacheEndpointTime;
};

class EndpointManager : public QueryEpBase
{
public:
	static const size_t iMinWeightLimit = 10;
	static const size_t iMaxWeightLimit = 100;
	static const int iCpuInvalidLimit = 3;
	static const int iCpuLoadMinLimit = 30;
	static const int iCpuLoadThreshold = 50;

public:
    EndpointManager(ObjectProxy* pObjectProxy,Communicator* pComm,const string & sObjName,bool bFirstNetThread,const string& setName="");

    virtual ~EndpointManager();

    //void init(const string& sObjName);

	/**
	 * 根据请求策略从可用的服务列表选择一个服务节点
	 */
    bool selectAdapterProxy(ReqMessage * msg,AdapterProxy * & pAdapterProxy);

    const vector<AdapterProxy*> & getAdapters()
    {
        return _vAllProxys;
    }

public:

    void notifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive,bool bSync=false);

    void doNotify();

public:

    //从指定的一组adapter中选取一个有效的
    AdapterProxy * selectFromGroup(ReqMessage * msg);

private:

    //轮询选取一个结点
    AdapterProxy * getNextValidProxy();

    //根据hash值选取一个结点
    AdapterProxy* getHashProxy(int64_t hashCode, bool bConsistentHash = false);

	//根据hash值从正常节点中选取一个结点
	AdapterProxy* getHashProxyForNormal(int64_t hashCode);

	AdapterProxy* getConHashProxyForNormal(int64_t hashCode);

	AdapterProxy* getHashProxyForWeight(int64_t hashCode, bool bStatic, vector<size_t> &vRouterCache);

	AdapterProxy* getConHashProxyForWeight(int64_t hashCode, bool bStatic);

	bool checkHashStaticWeightChange(bool bStatic);

	bool checkConHashChange(bool bStatic, const vector<AdapterProxy*> &vLastConHashProxys);

	void updateHashProxyWeighted(bool bStatic);

	void updateConHashProxyWeighted(bool bStatic, vector<AdapterProxy*> &vLastConHashProxys, TC_ConsistentHashNew &conHash);

	//根据hash值从设置静态权重的正常节点中选取一个结点
	//AdapterProxy* getHashProxyForStaticWeight(int64_t hashCode);

	//从灰度节点中选取一个结点
	AdapterProxy* getHashProxyForGrid(int64_t hashCode);

	//根据后端服务的负载值选取一个结点
	AdapterProxy* getNextWeightedProxy();

    //更新节点权值区间与节点索引映射表
    void updateWeightIndex(const map<int,size_t>& mLoadedIndex);

	//根据后端服务的权重值选取一个结点(其中灰度节点采用轮询方法，正常节点才用权重调用)
	AdapterProxy* getWeightedProxy(bool bStaticWeighted);

	//根据后端服务的权重值选取一个结点(正常节点的权重调用)
	AdapterProxy* getWeightedForNormal(bool bStaticWeighted);

	//根据后端服务的权重值选取一个结点(灰度节点采用轮询方法)
	AdapterProxy* getWeightedForGrid();

	//根据各个节点的权重值，建立各个节点的调用数
	void updateProxyWeighted();

	//根据各个节点的静态权重值，建立各个节点的静态调用数
	void updateStaticWeighted();

	//根据各个节点的成功率和平均延时，建立各个节点的调用数
	void updateDynamicWeighted();

	void updateDynamicWeightInfo(bool bCpuLoadValid = true, bool bDirect = false);

	void dispatchEndpointCache(const vector<int> &vWeight, bool bDynamic);

	//动态权重信息刷新到缓存文件
    void setDynamicEndpointToCache(bool bInactive, const set<EndpointInfo> & sEndpoints);

private:

    ObjectProxy * _pObjectProxy;

private:
    size_t _iLastRoundPosition;

	size_t _iGridLastRoundPosition;

    //活跃节点
	vector<AdapterProxy*> _vLoadedActiveProxy;

	/**
	 * 节点权值区间与节点索引映射表
	 */
    map<size_t,int>  _mWeightIndex;

	/**
	 * 上次重新映射表的时间
	 */
	int64_t _iLastBuildMapTime;

	/**
	 * 负载值更新频率,单位毫秒
	 */
    int32_t _UpdateLoadedInterval;

private:

	//节点信息是否有更新
	bool					_bUpdate;

	//是否是第一次建立动态权重信息
	bool					_bFirst;

	//连续采集到的cpu信息不正常的次数
	int						_iCpuLoadInvalidNum;

	/**
	 * 上次重新映射表的时间
	 */
	int64_t					_iLastBuildWeightTime;

	/**
	 * 负载值更新频率,单位毫秒
	 */
    int32_t					_iUpdateWeightInterval;

	/**
	 * 静态时，对应的节点路由选择
	 */
	size_t					_iLastSWeightPosition;

	vector<size_t>			_vStaticRouterCache;

	vector<size_t>			_vHashStaticRouterCache;

	/**
	 * 动态时，对应的节点路由选择
	 */
	size_t					_iLastDWeightPosition;

	vector<size_t>			_vDynamicRouterCache;

	//vector<size_t>			_vHashDynamicRouterCache;
	
	//活跃节点
	vector<AdapterProxy*>	_vActiveWeightProxy;

	//hash静态权重时使用
	vector<AdapterProxy*>   _vLastHashStaticProxys;

	//hash动态权重时使用
	//vector<AdapterProxy*>   _vLastHashDynamicProxys;

	//不活跃节点列表,含有上一次的权重信息，不含cpu负载等信息
    set<string>				_sLastInactiveEndpoints;

    //活跃节点列表,含有上一次的权重信息，不含cpu负载等信息
    set<string>				_sLastActiveEndpoints;

	map<int, int>			_mRatioToWeight;

	//一致性hash静态权重时使用
	vector<AdapterProxy*>   _vLastConHashWeightProxys;
	//一致性hash静态权重时使用
	TC_ConsistentHashNew	_consistentHashWeight;

	//一致性hash普通使用
	vector<AdapterProxy*>   _vLastConHashProxys;
	//一致性hash普通使用
	TC_ConsistentHashNew	_consistentHash;
	
private:

	//轮询调用数
	size_t				  _invokeNum;

	//hash调用数
	size_t				  _invokeHashNum;

	//权重调用数
	size_t				  _invokeWeightedNum;

	//正常调用达到这个次数后，可以进行灰度调用，目前按1/100灰度调用
	size_t				  _invokeGridRatioNum;
	size_t				  _invokeGridRatioIncrement;

    //活跃的结点
    vector<AdapterProxy*> _vActiveProxys;
	
    //部署的结点 包括活跃的和不活跃的
    map<string,AdapterProxy*> _regProxys;
    vector<AdapterProxy*> _vRegProxys;

	//灰度的活跃节点
	vector<AdapterProxy*> _vActiveGridProxys;

	//灰度部署的结点 包括活跃的和不活跃的
    map<string,AdapterProxy*> _regGridProxys;
    vector<AdapterProxy*> _vRegGridProxys;

    //所有曾经create的结点
    map<string,AdapterProxy*> _allProxys;
    vector<AdapterProxy*> _vAllProxys;
};

//一个type_obj 的对象
class EndpointThread : public QueryEpBase
{

public:
    EndpointThread(Communicator* pComm,const string & sObjName,GetEndpointType type,const string & sSetName,bool bFirstNetThread=false);
	~EndpointThread(){};
public:

    void getEndpoints(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);
    void getTCEndpoints(vector<TC_Endpoint> &activeEndPoint, vector<TC_Endpoint> &inactiveEndPoint);

    void notifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive,bool bSync);
    //虚函数 用不到 直接用空函数
    void doNotify()
    {
    }

private:

    //活跃的结点
    vector<EndpointInfo> _vActiveEndPoint;
    vector<TC_Endpoint> _vActiveTCEndPoint;

    //不活跃的结点
    vector<EndpointInfo> _vInactiveEndPoint;
    vector<TC_Endpoint> _vInactiveTCEndPoint;

    TC_ThreadLock _lock;

    string _sName;
    GetEndpointType _type;
};

//对外接口请求的class
class EndpointManagerThread
{
    public:
        EndpointManagerThread(Communicator * pComm,const string & sObjName);
		~EndpointManagerThread();
    public:
        //获取默认的结点
        void getEndpoint(vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //获取所有的结点
        void getEndpointByAll(vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //根据set获取结点
        void getEndpointBySet(const string sName,
                vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //根据地区获取结点
        void getEndpointByStation(const string sName,
                vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);


        //获取默认的结点
        void getTCEndpoint( vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //获取所有的结点
        void getTCEndpointByAll(vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //根据set获取结点
        void getTCEndpointBySet(const string sName,
                vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //根据地区获取结点
        void getTCEndpointByStation(const string sName,
                vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

    protected:
        EndpointThread * getEndpointThread(GetEndpointType type,const string & sName);
    private:
        Communicator * _pCommunicator;

        //保存对象的map
        map<string,EndpointThread*> _mInfo;

        string _sObjName;

        //lock
        TC_ThreadLock _lock;
};

////////////////////////////////////////////////////////////////////////
}
#endif
