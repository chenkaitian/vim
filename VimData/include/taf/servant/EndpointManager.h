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
	static string WEIGHT_SUCC_RATIO;  //�ɹ���ռ���ַ�������

    static string WEIGHT_AVGTIME_RATIO;  //ƽ����ʱռ���ַ�������

    static string WEIGHT_CPULOAD_RATIO; //cpu����ռ���ַ�������

	static string WEIGHT_ADJUST_THRESHOLD; //Ȩ�ص��ڷ�ֵ

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

    //���������������� �ʼ�������� Ҫ֪ͨ
    virtual void doNotify() = 0;

    //��ʼ���ӿ� ����obj����������
    bool init(const string & sObjName,const string & sLocator,const string& setName="");
protected:

    //ˢ������
    void refreshReg(GetEndpointType type,const string & sName);

private:
    void setEndpoints(const string & sEndpoints, set<EndpointInfo> & setEndpoints, set<string> & sEndpointInfo);

    //����obj����,����Ǽ�����ӣ����������ش������ӻ����м�����Ӧ���б�
    void setObjName(const string & sObjName);

    void doEndpoints(const vector<taf::EndpointF>& activeEp, const vector<taf::EndpointF>& inactiveEp,int iRet,bool bSync=false);
    void doEndpointsExp(int iRet,bool bSync = false);

    //������Ϣˢ�µ������ļ�
    void setEndPointToCache(bool bInactive);

	void getWeightRatio(const map<std::string, std::string> & mRspContext);

protected:

    //�Ƿ�ֱ����ʶ
    bool _bDirectProxy;

    //��ʾ�Ƿ�ӿڵ�����
    bool _bThread;

	//��һ��ʹ��cache��ȡip�б�
	bool _bFirstUseCache;

	//��������ȡ�Ľڵ���Ϣ���ϴ��Ƿ�һ��
	bool _bSameEndpoints;

	//�ɼ�����cpu��Ϣ�Ƿ�����
	bool _bCpuLoadValid;

	//Ȩ������
	EndpointWeightType _weightType;

	//�ɹ���ռ��
	size_t _iSuccRatio;

	//ƽ����ʱռ��
	size_t _iAvgTimeRatio;

	//cpu����ռ��
	size_t _iCpuLoadRatio;

	//Ȩ�ص��ڷ�ֵ
	int	_iWeightAdjustThreshold;

    //obj name
    string _sObjName;

	//ָ��set���õ�setid,Ĭ��Ϊ�գ������ֵ����˵����ָ��set����
	string _invokeSetId;

	QueryFPrx _queryFPrx;

    Communicator * _pCommunicator;

	//��Ծ�ڵ��б���Ϣ������cpu���ص���Ϣ
    set<string> _sActiveEndpoints;

    //����Ծ�ڵ��б� �ڹ���ƽ̨���滹���еģ�����cpu���ص���Ϣ
    set<string> _sInactiveEndpoints;

    //��Ծ�ڵ��б�
    set<EndpointInfo> _activeEndpoints;

    //����Ծ�ڵ��б� �ڹ���ƽ̨���滹���е�
    set<EndpointInfo> _inactiveEndpoints;

    //�����Ƿ���Ч��ֻ����������ػ��ߴ��ļ�load���������ݲ�����Ч����
    //��ʼ����ʱ������Ч������
    bool _valid;


    //�Ƿ��һ�������߳�
    bool _bFirstNetThread;

	//���ص�ַ
	string _sLocator;
private:

    //�����б��Ƶ��,��λ��
    int _iRefreshInterval;

    //����ʧ�ܵļ��ʱ��
    int _iFailInterval;

    //���������Ծ�б�Ϊ�յļ��ʱ��
    int _iActiveEmptyInterval;

    //��������ʧ�ܵĴ���
    int _iMaxFailTimes;

    //�ﵽ��������ʧ�ܵĴ��� �Ժ��ʱ����
    int _iManyFailInterval;

    //�������صĳ�ʱʱ��
    int _iTimeOut;

    //�������صĵȴ�ʱ��
    int _iWaitTime;

    //����ĳ�ʱʱ��(����ʱ��) ��ֹ����ص����ˡ�һֱ����������״̬
    int64_t _iRequestTimeout;

    //�Ƿ���������
    bool _bRequest;

    //����ʧ�ܵĴ���
    int _iFailTimes;

    //�´θ���ʱ��,��λ��
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
	 * ����������Դӿ��õķ����б�ѡ��һ������ڵ�
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

    //��ָ����һ��adapter��ѡȡһ����Ч��
    AdapterProxy * selectFromGroup(ReqMessage * msg);

private:

    //��ѯѡȡһ�����
    AdapterProxy * getNextValidProxy();

    //����hashֵѡȡһ�����
    AdapterProxy* getHashProxy(int64_t hashCode, bool bConsistentHash = false);

	//����hashֵ�������ڵ���ѡȡһ�����
	AdapterProxy* getHashProxyForNormal(int64_t hashCode);

	AdapterProxy* getConHashProxyForNormal(int64_t hashCode);

	AdapterProxy* getHashProxyForWeight(int64_t hashCode, bool bStatic, vector<size_t> &vRouterCache);

	AdapterProxy* getConHashProxyForWeight(int64_t hashCode, bool bStatic);

	bool checkHashStaticWeightChange(bool bStatic);

	bool checkConHashChange(bool bStatic, const vector<AdapterProxy*> &vLastConHashProxys);

	void updateHashProxyWeighted(bool bStatic);

	void updateConHashProxyWeighted(bool bStatic, vector<AdapterProxy*> &vLastConHashProxys, TC_ConsistentHashNew &conHash);

	//����hashֵ�����þ�̬Ȩ�ص������ڵ���ѡȡһ�����
	//AdapterProxy* getHashProxyForStaticWeight(int64_t hashCode);

	//�ӻҶȽڵ���ѡȡһ�����
	AdapterProxy* getHashProxyForGrid(int64_t hashCode);

	//���ݺ�˷���ĸ���ֵѡȡһ�����
	AdapterProxy* getNextWeightedProxy();

    //���½ڵ�Ȩֵ������ڵ�����ӳ���
    void updateWeightIndex(const map<int,size_t>& mLoadedIndex);

	//���ݺ�˷����Ȩ��ֵѡȡһ�����(���лҶȽڵ������ѯ�����������ڵ����Ȩ�ص���)
	AdapterProxy* getWeightedProxy(bool bStaticWeighted);

	//���ݺ�˷����Ȩ��ֵѡȡһ�����(�����ڵ��Ȩ�ص���)
	AdapterProxy* getWeightedForNormal(bool bStaticWeighted);

	//���ݺ�˷����Ȩ��ֵѡȡһ�����(�ҶȽڵ������ѯ����)
	AdapterProxy* getWeightedForGrid();

	//���ݸ����ڵ��Ȩ��ֵ�����������ڵ�ĵ�����
	void updateProxyWeighted();

	//���ݸ����ڵ�ľ�̬Ȩ��ֵ�����������ڵ�ľ�̬������
	void updateStaticWeighted();

	//���ݸ����ڵ�ĳɹ��ʺ�ƽ����ʱ�����������ڵ�ĵ�����
	void updateDynamicWeighted();

	void updateDynamicWeightInfo(bool bCpuLoadValid = true, bool bDirect = false);

	void dispatchEndpointCache(const vector<int> &vWeight, bool bDynamic);

	//��̬Ȩ����Ϣˢ�µ������ļ�
    void setDynamicEndpointToCache(bool bInactive, const set<EndpointInfo> & sEndpoints);

private:

    ObjectProxy * _pObjectProxy;

private:
    size_t _iLastRoundPosition;

	size_t _iGridLastRoundPosition;

    //��Ծ�ڵ�
	vector<AdapterProxy*> _vLoadedActiveProxy;

	/**
	 * �ڵ�Ȩֵ������ڵ�����ӳ���
	 */
    map<size_t,int>  _mWeightIndex;

	/**
	 * �ϴ�����ӳ����ʱ��
	 */
	int64_t _iLastBuildMapTime;

	/**
	 * ����ֵ����Ƶ��,��λ����
	 */
    int32_t _UpdateLoadedInterval;

private:

	//�ڵ���Ϣ�Ƿ��и���
	bool					_bUpdate;

	//�Ƿ��ǵ�һ�ν�����̬Ȩ����Ϣ
	bool					_bFirst;

	//�����ɼ�����cpu��Ϣ�������Ĵ���
	int						_iCpuLoadInvalidNum;

	/**
	 * �ϴ�����ӳ����ʱ��
	 */
	int64_t					_iLastBuildWeightTime;

	/**
	 * ����ֵ����Ƶ��,��λ����
	 */
    int32_t					_iUpdateWeightInterval;

	/**
	 * ��̬ʱ����Ӧ�Ľڵ�·��ѡ��
	 */
	size_t					_iLastSWeightPosition;

	vector<size_t>			_vStaticRouterCache;

	vector<size_t>			_vHashStaticRouterCache;

	/**
	 * ��̬ʱ����Ӧ�Ľڵ�·��ѡ��
	 */
	size_t					_iLastDWeightPosition;

	vector<size_t>			_vDynamicRouterCache;

	//vector<size_t>			_vHashDynamicRouterCache;
	
	//��Ծ�ڵ�
	vector<AdapterProxy*>	_vActiveWeightProxy;

	//hash��̬Ȩ��ʱʹ��
	vector<AdapterProxy*>   _vLastHashStaticProxys;

	//hash��̬Ȩ��ʱʹ��
	//vector<AdapterProxy*>   _vLastHashDynamicProxys;

	//����Ծ�ڵ��б�,������һ�ε�Ȩ����Ϣ������cpu���ص���Ϣ
    set<string>				_sLastInactiveEndpoints;

    //��Ծ�ڵ��б�,������һ�ε�Ȩ����Ϣ������cpu���ص���Ϣ
    set<string>				_sLastActiveEndpoints;

	map<int, int>			_mRatioToWeight;

	//һ����hash��̬Ȩ��ʱʹ��
	vector<AdapterProxy*>   _vLastConHashWeightProxys;
	//һ����hash��̬Ȩ��ʱʹ��
	TC_ConsistentHashNew	_consistentHashWeight;

	//һ����hash��ͨʹ��
	vector<AdapterProxy*>   _vLastConHashProxys;
	//һ����hash��ͨʹ��
	TC_ConsistentHashNew	_consistentHash;
	
private:

	//��ѯ������
	size_t				  _invokeNum;

	//hash������
	size_t				  _invokeHashNum;

	//Ȩ�ص�����
	size_t				  _invokeWeightedNum;

	//�������ôﵽ��������󣬿��Խ��лҶȵ��ã�Ŀǰ��1/100�Ҷȵ���
	size_t				  _invokeGridRatioNum;
	size_t				  _invokeGridRatioIncrement;

    //��Ծ�Ľ��
    vector<AdapterProxy*> _vActiveProxys;
	
    //����Ľ�� ������Ծ�ĺͲ���Ծ��
    map<string,AdapterProxy*> _regProxys;
    vector<AdapterProxy*> _vRegProxys;

	//�ҶȵĻ�Ծ�ڵ�
	vector<AdapterProxy*> _vActiveGridProxys;

	//�ҶȲ���Ľ�� ������Ծ�ĺͲ���Ծ��
    map<string,AdapterProxy*> _regGridProxys;
    vector<AdapterProxy*> _vRegGridProxys;

    //��������create�Ľ��
    map<string,AdapterProxy*> _allProxys;
    vector<AdapterProxy*> _vAllProxys;
};

//һ��type_obj �Ķ���
class EndpointThread : public QueryEpBase
{

public:
    EndpointThread(Communicator* pComm,const string & sObjName,GetEndpointType type,const string & sSetName,bool bFirstNetThread=false);
	~EndpointThread(){};
public:

    void getEndpoints(vector<EndpointInfo> &activeEndPoint, vector<EndpointInfo> &inactiveEndPoint);
    void getTCEndpoints(vector<TC_Endpoint> &activeEndPoint, vector<TC_Endpoint> &inactiveEndPoint);

    void notifyEndpoints(const set<EndpointInfo> & active,const set<EndpointInfo> & inactive,bool bSync);
    //�麯�� �ò��� ֱ���ÿպ���
    void doNotify()
    {
    }

private:

    //��Ծ�Ľ��
    vector<EndpointInfo> _vActiveEndPoint;
    vector<TC_Endpoint> _vActiveTCEndPoint;

    //����Ծ�Ľ��
    vector<EndpointInfo> _vInactiveEndPoint;
    vector<TC_Endpoint> _vInactiveTCEndPoint;

    TC_ThreadLock _lock;

    string _sName;
    GetEndpointType _type;
};

//����ӿ������class
class EndpointManagerThread
{
    public:
        EndpointManagerThread(Communicator * pComm,const string & sObjName);
		~EndpointManagerThread();
    public:
        //��ȡĬ�ϵĽ��
        void getEndpoint(vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //��ȡ���еĽ��
        void getEndpointByAll(vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //����set��ȡ���
        void getEndpointBySet(const string sName,
                vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);

        //���ݵ�����ȡ���
        void getEndpointByStation(const string sName,
                vector<EndpointInfo> &activeEndPoint,
                vector<EndpointInfo> &inactiveEndPoint);


        //��ȡĬ�ϵĽ��
        void getTCEndpoint( vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //��ȡ���еĽ��
        void getTCEndpointByAll(vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //����set��ȡ���
        void getTCEndpointBySet(const string sName,
                vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

        //���ݵ�����ȡ���
        void getTCEndpointByStation(const string sName,
                vector<TC_Endpoint> &activeEndPoint,
                vector<TC_Endpoint> &inactiveEndPoint);

    protected:
        EndpointThread * getEndpointThread(GetEndpointType type,const string & sName);
    private:
        Communicator * _pCommunicator;

        //��������map
        map<string,EndpointThread*> _mInfo;

        string _sObjName;

        //lock
        TC_ThreadLock _lock;
};

////////////////////////////////////////////////////////////////////////
}
#endif
