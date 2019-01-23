#ifndef __TAF_OBJECT_PROXY_H_
#define __TAF_OBJECT_PROXY_H_

#include "servant/Communicator.h"
#include "servant/Message.h"
#include "servant/AdapterProxy.h"
#include "servant/EndpointInfo.h"
#include "servant/EndpointF.h"
#include "servant/AppProtocol.h"
#include "servant/Global.h"
#include "util/tc_timeout_queue_noid.h"

namespace taf
{
class EndpointManager;

struct SocketOpt
{
	int level;

	int optname;

	const void *optval;

	socklen_t optlen;
};

///////////////////////////////////////////////////////////////////
/**
 * 每个objectname在每个客户端网络线程中有唯一一个objectproxy
 * 管理收发的消息队列
 */
class ObjectProxy : public TC_HandleBase, public TC_ThreadMutex //public BaseProxy
{
public:
    /**
     * 构造函数
     * @param comm
     * @param name
     * @param setName 指定set调用的setid
     */
    ObjectProxy(CommunicatorEpoll * pCommunicatorEpoll, const string & sObjectProxyName,const string& setName="");

    /**
     * 析构函数
     */
    ~ObjectProxy();

    /**
     * 初始化消息队列等
     */
    void initialize();

    /**
     * 获取servantproxy
     */
    ServantProxy * getServantProxy();

    /**
     * 设置servantproxy
     */
    void setServantProxy(ServantProxy * pServantProxy);

    /**
     * 方法调用
     * @param req
     * @return int
     */
    void invoke(ReqMessage *  msg);

    /**
     * 发送积攒在obj的数据
     */
    void doInvoke();

    /**
     * 获取连接超时时间
     * @return int
     */
    int getConTimeout()
    {
        return _conTimeout;
    }

    /**
     * 设置连接超时时间
     * @return int
     */
    void setConTimeout(int conTimeout)
    {
        _conTimeout = conTimeout;
    }

    /**
     * 获取object名称
     * @return const string&
     */
    const string & name() const;

    /**
     * 超时策略获取和设置
     * @return CheckTimeoutInfo&
     */
    CheckTimeoutInfo& checkTimeoutInfo();

    /**
     * 设置协议解析器
     * @return UserProtocol&
     */
    void setProxyProtocol(const ProxyProtocol& protocol);

    /**
     * 获取协议解析器
     * @return ProxyProtocol&
     */
    ProxyProtocol& getProxyProtocol();

    /**
    *设置套接口选项
    */
    void setSocketOpt(int level, int optname, const void *optval, socklen_t optlen);

     /**
     *获取套接字选项
     */
     vector<SocketOpt>& getSocketOpt();

    /**
     * 设置刷新服务列表的间隔
     * @param msecond
     */
    void refreshEndpointInterval(int msecond);

     /**
     * 设置缓存服务列表的间隔
     * @param msecond
     */
    void cacheEndpointInterval(int msecond);

    /**
     * 加载locator
     * @param
     * @return AdapterPrx
     */
    int loadLocator();

    /**
     * 设置PUSH类消息的callback对象
     * @param cb
     */
    void setPushCallbacks(const ServantProxyCallbackPtr& cb);

    /**
     * 获取PUSH类消息的callback对象
     */
    inline ServantProxyCallbackPtr & getPushCallback()
    {
        return _pPushCallback;
    }

    uint32_t generateId()
    {
        _uiId ++;
        if(_uiId == 0)
            ++_uiId;
        return _uiId;
    }

    /**
     * 获取所有的adapter
     */
    const vector<AdapterProxy*> & getAdapters() const;

    /**
     * 检查Obj的队列超时
     */
    void doTimeout();

    /**
     * Obj的队列的长度
     */
    size_t timeQSize()
    {
        return _reqInfoTimeQ.size();
    }
	/**
	 * 判断此obj是否走按set规则调用流程，如果是直连方式，即使服务端是启用set的，也不认为是按set规则调用的
	 */
	bool IsInvokeBySet() const;

	/**
	 * 获取按set规则调用的set名称
	 */
	const string& getInvokeSetName() const;

	/**
	 * 获取鉴权函数
	 */
	auth_req_functor& getAuthReqFunc() { return _authReqf; }
	/**
	 * 设置鉴权函数
	 */
	void setAuthReqFunc(const auth_req_functor& f) { _authReqf = f; }
	/**
	 * 获取鉴权响应函数
	 */
	auth_rsp_functor& getAuthRspFunc() { return _authRspf; }
	/**
	 * 设置鉴权响应函数
	 */
	void setAuthRspFunc(const auth_rsp_functor& f) { _authRspf = f; }
    /**
     * 设置AK
     */
    void setAccessKey(const string& k) { _accessKey = k; }
    /**
     * 设置SK
     */
    void setSecretKey(const string& k) { _secretKey = k; }
    /**
     * 获取AK
     */
    const string& getAccessKey() const  { return _accessKey; }
    /**
     * 获取SK
     */
    const string& getSecretKey() const  { return _secretKey; }
public:

    int timeout() const
    {
        return _timeout;
    }

    void timeout(int msec)
    {
        //保护，异步超时时间不能小于100毫秒
        if (msec >= 100)
            _timeout = msec;
        else
            _timeout = 100;
    }

    inline CommunicatorEpoll * getCommunicatorEpoll()
    {
        return _pCommunicatorEpoll;
    }

	/**
     * 设置调用成功率占比和平均耗时占比的值
     */
	inline void setSuccAndAvgTimeRatio(size_t iSuccRatio, size_t iAvgTimeRatio) 
	{ 
		_succRatio = iSuccRatio;
		_avgTimeRatio = iAvgTimeRatio;
	}

	/**
     * 获取调用成功率占比的值
     */
	inline size_t getSuccRatio() { return _succRatio; }

	/**
     * 获取调用平均耗时占比的值
     */
	inline size_t getAvgTimeRatio() { return _avgTimeRatio; }

protected:

	/**
	 * 处理请求异常
	 *
	 */
	void doInvokeException(ReqMessage * msg);

private:

    uint32_t _uiId;

    string _name;

	//按set规则调用的set名称
	string _invokeSetId;

	//是否是按set规则调用
	bool  _bInvokeBySet;

	//是否调用了taf_set_protocol设置过proxy的协议函数，设置过了就不在设置
	bool  _bProcotolSet;

    string _locator;

    int32_t _timeout;

    int32_t _maxTransNum;

    string _queryFObjectName;

    bool _isDirectProxy;

    bool _serverHasGrid;

    int32_t _refreshEndpointInterval;

    time_t _lastCacheEndpointTime;

    int32_t _cacheEndpointInterval;

    int32_t _roundStartIndex;

    int _conTimeout;

    CheckTimeoutInfo _checkTimeoutInfo;

	size_t	_succRatio;

	size_t  _avgTimeRatio;

    ProxyProtocol _proxyProtocol;

    vector<SocketOpt> _SocketOpts;

    //push消息 callback
    ServantProxyCallbackPtr _pPushCallback;

    //结点管理类
    EndpointManager * _pEndpointManger;

    CommunicatorEpoll * _pCommunicatorEpoll;

    TC_TimeoutQueueNoID<ReqMessage *> _reqInfoTimeQ;

    ServantProxy * _pServantProxy;

	/**
     * 鉴权打包函数
	 */
    auth_req_functor _authReqf;
	/**
     * 鉴权响应函数
	 */
    auth_rsp_functor _authRspf;
    /**
     * AK/SK
     */
    std::string     _accessKey;
    std::string     _secretKey;
};
///////////////////////////////////////////////////////////////////////////////////
}
#endif
