#ifndef __TAF_ENDPOINT_INFO_H_
#define __TAF_ENDPOINT_INFO_H_

#include "servant/Global.h"
#include "servant/NetworkUtil.h"

using namespace std;

namespace taf
{
/**
 * 地址信息IP:Port
 */
class EndpointInfo
{
public:
	enum EType { TCP = 1, UDP = 2, };

    /**
     * 构造函数
     */
	EndpointInfo();

    /**
     * 构造函数
     * @param host
     * @param port
     * @param type
     */
	EndpointInfo(const string& host, uint16_t port, EndpointInfo::EType type, int32_t grid, const string & setDivision, int qos, int gridStatus = 0, int weight = -1, unsigned int weighttype = 0, int cpuload = -1, int64_t sampletime = 0, int weightflow = -1);

	/**
	* 构造函数
	* @param host
	* @param port
	* @param type
	* @param container
	*/
	EndpointInfo(const string& host, uint16_t port, EndpointInfo::EType type, int32_t grid, const string & setDivision, int qos, string sContainer, int gridStatus = 0, int weight = -1, unsigned int weighttype = 0, int cpuload = -1, int64_t sampletime = 0, int weightflow = -1);

    /**
     * 地址的字符串描述,不带set信息
     *
     * @return string
     */
     const string& descNoSetInfo() const;

    /**
     * 地址的字符串描述
     *
     * @return string
     */
	const string & desc() const
    {
        return _desc;
    }

	const string & compareDesc() const
    {
        return _sCompareDesc;
    }

	const string & compareDescNoCpu() const
    {
        return _sCompareDescNoCpu;
    }

    /**
     * 详细地址字符串描述
     * 
     * @return string 
     */
	const string& fulldesc() const;

    /**
     * 获取主机名
     *
     * @return const string&
     */
	string host() const;

    /**
     * 获取端口号
     *
     * @return uint16_t
     */
	uint16_t port() const;

    /**
     * 获取路由状态
     * @return int32_t
     */
	int32_t grid() const;

    /*
     * 获取qos的descp值
     */
    int32_t qos() const {return _qos;}

	/*
     * 获取灰度状态值
     */
    int32_t gridStatus() const {return _gridStatus;}

	/*
     * 获取节点的静态权重值
     */
    int weight() const {return _weight;}

	/**
     * @brief 获取节点的权重使用方式
     */
    unsigned int getWeightType() const { return _weighttype; }

	/**
     * @brief 获取节点的cpu负载
     */
	int cpuLoad() const {return _cpuload;}

	/**
     * @brief 获取节点cpu负载的采样时间
     */
	int64_t sampleTime() const {return _sampletime;}

	/**
     * @brief 获取节点的权重流量
     */
	int weightFlow() const {return _weightflow;}

	/**
     * @brief 设置节点的权重流量
     */
	void setWeightFlow(int iWeightFlow) { _weightflow = iWeightFlow; }

    /**
     * 获取主机地址
     *
     * @return const struct sockaddr_in&
     */
	const struct sockaddr_in& addr() const;

    /**
     * 返回端口类型
     *
     * @return EndpointInfo::EType
     */
	EndpointInfo::EType type() const;

    /**
    *设置set分组信息
    *
    *@return void
    */
    void setDivision(const string& sSetDivision);

    /**
    *返回set分组信息
    *
    *@return string
    */
    const string& setDivision() const;

	string createCacheDesc() const;

	/*
	* 获取容器名
	*/
	string getContainer() const
	{
		return _sContainer;
	}

#if 0
    /**
     * 赋值方法
     */
    EndpointInfo& operator = (const EndpointInfo& r);
#endif

    /**
     * 等于
     * @param r
     *
     * @return bool
     */
    bool operator == (const EndpointInfo& r) const;

    /**
    *等于,set信息不参与比较
    *@param r
    *
    *@return bool
    */
    bool equalNoSetInfo(const EndpointInfo& r) const;

    /**
     * 小于
     * @param r
     *
     * @return bool
     */
    bool operator < (const EndpointInfo& r) const;

protected:

    /**
     * 地址的字符串描述
     * @param bWithSetInfo,标识
     * @return string
     */
	string createDesc() const;

    /**
     * 详细地址字符串描述
     * 
     * @return string 
     */
	string createCompareDesc();

	string getEndpointInfoDesc();

	

private:
    /**
     * 地址IP
     */
    char _host[16];

    /**
     * 端口号
     */
    uint16_t _port;

    /**
     * 路由状态
     */
    int32_t _grid;

    /*
     * qos的dscp值
     */

    int32_t _qos;

    /**
     * 类型
     */
	EndpointInfo::EType _type;

   /**
    *set分组信息
    */
    string _setDivision;

    /**
     * 地址
     */
	struct sockaddr_in _addr;

    string _sCompareDesc;

	string _sCompareDescNoCpu;

    /**
     * 地址描述
     */
    string _desc;

	/**
     * 节点状态，分为正常、灰度、无流量
     */
	int32_t	_gridStatus;

	/**
     *  节点的静态权重值
     */
	int _weight;

	/**
     *  节点的权重使用方式
     */
	unsigned int	_weighttype;

	/**
     *  节点的cpu负载信息
     */
	int _cpuload;

	/**
     *  采样cpu负载信息的时间点
     */
	int64_t _sampletime;

	/**
     *  节点权重流量
     */
	int _weightflow;

	/*
	*容器名
	*/
	string _sContainer;

    #if 0
    /**
     * 地址全描述
     */
    string _fulldesc;


    /**
    *不带set信息的地址描述
    */
    string _descNoSetInfo;
	
    #endif
};
/////////////////////////////////////////////////////////////////////////////
}
#endif
