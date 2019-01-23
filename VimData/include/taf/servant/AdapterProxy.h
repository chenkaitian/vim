#ifndef __TAF_ADAPTER_PROXY_H_
#define __TAF_ADAPTER_PROXY_H_

#include "util/tc_timeout_queue_new.h"
#include "util/tc_timeout_queue_map.h"
#include "servant/Global.h"
#include "servant/EndpointInfo.h"
#include "servant/ObjectProxy.h"
#include "servant/Transceiver.h"
#include "servant/Message.h"
#include "servant/StatReport.h"
#include <queue>

#define ADAPTER_QUEUE_LEN 1000

namespace taf
{
/**
 * 每个Adapter对应一个Endpoint，也就是一个服务端口
 */
class AdapterProxy
{
public:
	struct StatMicWeightedMsg
	{
		int		total;
		int		succ;
		long	totalRspTime;
	};

	static const size_t iDynamicWeightValue = 100;

public:
    /**
     * 构造函数
     * @param ep
     * @param op
     */
    AdapterProxy(ObjectProxy * pObjectProxy, const EndpointInfo &ep,Communicator* pCom);

    /**
     * 析构函数
     */
    ~AdapterProxy();

    /**
     * 调用server端对象方法
     * @param req
     * @return int
     */
	int invoke(ReqMessage * msg);

    /**
     * 发送请求
     * 发送挤压的数据
     * @param req
     * @return
     */
    void doInvoke();

    /**
     * server端方法返回
     * @param req
     * @return int
     */
    void finishInvoke(ResponsePacket &rsp);

    void finishInvoke(ReqMessage * msg);

    /**
     * 端口是否有效,当连接全部失效时返回false
     * @bForceConnect : 是否强制发起连接,为true时不对状态进行判断就发起连接
     * @return bool
     */
    bool checkActive(bool bForceConnect = false);

	inline bool isConnTimeout() { return _bConnTimeout; }
    void setConTimeout(bool bConTimeout);

	inline bool isConnExc() { return _bConnExc;}
	inline int ConnExcCnt() const { return _connExcCnt;}

    int getConTimeout();

	/**
	 * 服务在住空的状态是否为active,不管是出问题导致，还是人为停止服务还是服务下线，都认为是inactive的
	 * @return bool
	 */
	inline bool isActiveInReg() {return _activeStateInReg;}

	inline void setActiveInReg(bool bActive) {_activeStateInReg = bActive;}
	inline size_t loaded() { return _loaded;}

    //inline void setRetrying(bool retrying) { _retrying = retrying;}

	/**
	 * 记录连接异常数
	 * @param bExc 连接是否出现异常
	 */
	void addConnExc(bool bExc);
    /**
     * 增加调用数
     * @param bTimeout, 是否是超时的
     */
    void finishInvoke(bool bTimeout);

    /**
     * 获取端口信息
     * @return const EndpointInfo&
     */
    const EndpointInfo & endpoint() const;

    /**
     * 从队列里取一个请求
     * @param trans
     * @return bool
     */
	bool sendRequest();

    /**
     * 获取连接
     *
     * @return Transceiver*
     */
    inline Transceiver* trans() { return _pTrans; }

	/**
	 * 如果需要异步连接服务，那么再次完成连接
	 */
	bool finishConnect();

    /**
     * 处理超时
     */
    void doTimeout();

    /**
     * 处理stat
     */
    void doStat(map<StatMicMsgHead, StatMicMsgBody> & mStatMicMsg);

    void sample(ReqMessage * msg);

    inline CommunicatorEpoll * getEpoll()
    {
        return _pCommunicatorEpoll;
    }

    ObjectProxy * getObjProxy();

	/**
     * 设置节点的灰度状态
     */
	void setGridFlag(bool bFlag) { _bGridFlag = bFlag; }

	/**
     * 获取节点的灰度状态
     */
	bool getGridFlag() { return _bGridFlag; }

	/**
     * 设置节点的静态权重值
     */
	inline void setWeight(int iWeight) { _iStaticWeight = iWeight; }

	/**
     * 获取节点的静态权重值
     */
	inline int getWeight() { return _iStaticWeight; }

	/**
     * 设置节点的cpu负载值
     */
	inline void setCpuLoad(int iCpuLoad) { _iCpuLoad = iCpuLoad; }

	/**
     * 获取节点的cpu负载值
     */
	inline int getCpuLoad() { return _iCpuLoad; }

	/**
     * 设置节点的上一次的权重值
     */
	inline void setLastWeight(int iLastWeight) { _iLastWeight = iLastWeight; }

	/**
     * 获取节点的上一次的权重值
     */
	inline int getLastWeight() { return _iLastWeight; }

	/**
     * 获取节点是否含有统计数据的状态标志
     */
	inline bool getHasStatData() { return _bHasStatData; }

	/**
     * 设置节点是否含有统计数据的状态标志
     */
	inline void setHasStatData(bool bFlag) { _bHasStatData = bFlag; }

	/**
     * 设置节点处于动态权重的状态标志
     */
	inline void setDynamicWeightFlag(bool bFlag) { _bDynamicFlag = bFlag; }

	/**
     * 清空计算动态权重的节点的调用信息
     */
	void clearWeightStat();

	/**
     * 获取节点的调用信息，以便计算动态权重
     */
	const AdapterProxy::StatMicWeightedMsg & getWeightStat() { return _mWeightedStat; }

	inline CommunicatorEpoll* getCommunicatorEpoll() { return _pCommunicatorEpoll;}

protected:

    /**
     * 设置服务的有效状态
     * @param value
     */
    void setActive(bool value);

    /**
     * 屏蔽结点
     */
    void setInactive();

    /**
     * 从队列中获取一个请求
     * @param req
     * @return bool
     */
	bool popRequest(ReqMessage * req);

    /**
     * 重连
     * @return TransceiverPtr
     */
    Transceiver* doReconnect();

private:

	void initStatHead();

	void stat(ReqMessage * msg);
    void merge(const StatMicMsgBody& inBody, StatMicMsgBody& outBody);
	//void merge2(const StatMicMsgBody& inBody, StatMicMsgBody& outBody,int64_t returnValue);

	string getSlaveName(const string& sSlaveName);

private:

	bool							_bDynamicFlag;

	bool							_bHasStatData;

	//静态权重值
	int								_iStaticWeight;

	int								_iCpuLoad;

	int								_iLastWeight;

	//调用的成功数和延时统计
	StatMicWeightedMsg				_mWeightedStat;

private:

    TC_TimeoutQueueNew<ReqMessage*>*   _pTimeoutQueue;

    ObjectProxy*                    _pObjectProxy;

    CommunicatorEpoll*              _pCommunicatorEpoll;

    Transceiver*                    _pTrans;

	Communicator*					_pCommunicator;

    EndpointInfo					_endpoint;

    int32_t							_checkTransInterval;

    uint32_t						_timeoutInvoke;

	uint32_t						_bConnTimeout;

    uint32_t						_totalInvoke;

    uint32_t						_frequenceFailInvoke;

    time_t                          _nextFinishInvokeTime;

    time_t                          _nextRetryTime;

    time_t							_lastCheckTransTime;

    time_t                          _frequenceFailTime;

    bool							_activeStatus;

    //bool                            _retrying;

	//该节点在主控的状态是否是active的,出故障和认为停止服务都认为是inactive的
	bool							_activeStateInReg;

	/**
	 * 连接异常个数,在时间片范围内(目前是1分钟)连续5次异常则认为连接失效
	 */
    uint32_t						_connExcCnt;
	bool							_bConnExc;


	//该节点服务端负载值
	size_t							_loaded;

    uint32_t                        _iMaxSampleCount;

    //生成模块间调用时序图的采样比率
    int                             _iSampleRate;

    //stat信息
    //key是函数名字
    map<string,StatMicMsgBody>		_mStatBody;

    //stat的头信息
	StatMicMsgHead					_mStatHead;

    //sample的信息
    map<string,vector<StatSampleMsg> >     _mSample;

	//正在自动扩容时设置有节点灰度的标识
	bool							_bGridFlag;

	//超时请求的回包回来后，是否打印超时的日志信息
	bool							_bTimeoutLogFlag;

	//非发送队列的大小限制，用于发送过载判断
	size_t							_sendQueueLimit;
};
////////////////////////////////////////////////////////////////////
}
#endif
