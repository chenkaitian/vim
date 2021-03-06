// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.2.2.11 by WSRD Tencent.
// Generated from `PropertyPlusF.jce'
// **********************************************************************

#ifndef __PROPERTYPLUSF_H_
#define __PROPERTYPLUSF_H_

#include <map>
#include <string>
#include <vector>
#include "jce/Jce.h"
#include "jce/wup.h"
#include "servant/BaseF.h"

using namespace wup;
using namespace std;

#include "servant/ServantProxy.h"
#include "servant/Servant.h"
#include "promise/promise.h"


namespace LogStat
{
    struct StatLog : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "LogStat.StatLog";
        }
        static string MD5()
        {
            return "2b230c3fd260b79338b0c09216c3d7e9";
        }
        StatLog()
        :logname(""),date(""),flag(""),extend(0)
        {
        }
        void resetDefautlt()
        {
            logname = "";
            date = "";
            flag = "";
            extend = 0;
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(logname, 0);
            _os.write(content, 1);
            _os.write(date, 2);
            _os.write(flag, 3);
            _os.write(extend, 4);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(logname, 0, true);
            _is.read(content, 1, true);
            _is.read(date, 2, false);
            _is.read(flag, 3, false);
            _is.read(extend, 4, false);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(logname,"logname");
            _ds.display(content,"content");
            _ds.display(date,"date");
            _ds.display(flag,"flag");
            _ds.display(extend,"extend");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(logname, true);
            _ds.displaySimple(content, true);
            _ds.displaySimple(date, true);
            _ds.displaySimple(flag, true);
            _ds.displaySimple(extend, false);
            return _os;
        }
    public:
        std::string logname;
        vector<std::string> content;
        std::string date;
        std::string flag;
        taf::Int32 extend;
    };
    inline bool operator==(const StatLog&l, const StatLog&r)
    {
        return l.logname == r.logname && l.content == r.content && l.date == r.date && l.flag == r.flag && l.extend == r.extend;
    }
    inline bool operator!=(const StatLog&l, const StatLog&r)
    {
        return !(l == r);
    }

    struct StatValue : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "LogStat.StatValue";
        }
        static string MD5()
        {
            return "995b2839f0317f4a610e9e80f716f79d";
        }
        StatValue()
        :policy(""),value(0),count(0)
        {
        }
        void resetDefautlt()
        {
            policy = "";
            value = 0;
            count = 0;
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(policy, 0);
            _os.write(value, 1);
            _os.write(count, 2);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(policy, 0, true);
            _is.read(value, 1, true);
            _is.read(count, 2, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(policy,"policy");
            _ds.display(value,"value");
            _ds.display(count,"count");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(policy, true);
            _ds.displaySimple(value, true);
            _ds.displaySimple(count, false);
            return _os;
        }
    public:
        std::string policy;
        taf::Float value;
        taf::Int64 count;
    };
    inline bool operator==(const StatValue&l, const StatValue&r)
    {
        return l.policy == r.policy && l.value == r.value && l.count == r.count;
    }
    inline bool operator!=(const StatValue&l, const StatValue&r)
    {
        return !(l == r);
    }

    struct StatContent : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "LogStat.StatContent";
        }
        static string MD5()
        {
            return "38b55f91190e7c5d2011aa842804bbdd";
        }
        StatContent()
        {
        }
        void resetDefautlt()
        {
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(keys, 0);
            _os.write(values, 1);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(keys, 0, true);
            _is.read(values, 1, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(keys,"keys");
            _ds.display(values,"values");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(keys, true);
            _ds.displaySimple(values, false);
            return _os;
        }
    public:
        vector<std::string> keys;
        vector<LogStat::StatValue> values;
    };
    inline bool operator==(const StatContent&l, const StatContent&r)
    {
        return l.keys == r.keys && l.values == r.values;
    }
    inline bool operator!=(const StatContent&l, const StatContent&r)
    {
        return !(l == r);
    }

    struct StatOneLog : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "LogStat.StatOneLog";
        }
        static string MD5()
        {
            return "ac5b95ee0e2ba4f533371a9fef2f0d1a";
        }
        StatOneLog()
        :logname("")
        {
        }
        void resetDefautlt()
        {
            logname = "";
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(logname, 0);
            _os.write(keys, 1);
            _os.write(values, 2);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(logname, 0, true);
            _is.read(keys, 1, true);
            _is.read(values, 2, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(logname,"logname");
            _ds.display(keys,"keys");
            _ds.display(values,"values");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(logname, true);
            _ds.displaySimple(keys, true);
            _ds.displaySimple(values, false);
            return _os;
        }
    public:
        std::string logname;
        vector<std::string> keys;
        vector<LogStat::StatValue> values;
    };
    inline bool operator==(const StatOneLog&l, const StatOneLog&r)
    {
        return l.logname == r.logname && l.keys == r.keys && l.values == r.values;
    }
    inline bool operator!=(const StatOneLog&l, const StatOneLog&r)
    {
        return !(l == r);
    }

    /* callback of async proxy for client */
    class PropertyPlushFPrxCallback: public taf::ServantProxyCallback
    {
    public:
        virtual ~PropertyPlushFPrxCallback(){}
        virtual void callback_logstat(taf::Int32 ret)
        { throw std::runtime_error("callback_logstat() override incorrect."); }
        virtual void callback_logstat_exception(taf::Int32 ret)
        { throw std::runtime_error("callback_logstat_exception() override incorrect."); }

        virtual void callback_mutillogstat(taf::Int32 ret)
        { throw std::runtime_error("callback_mutillogstat() override incorrect."); }
        virtual void callback_mutillogstat_exception(taf::Int32 ret)
        { throw std::runtime_error("callback_mutillogstat_exception() override incorrect."); }

    public:
        virtual const map<std::string, std::string> & getResponseContext() const;

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    };
    typedef taf::TC_AutoPtr<PropertyPlushFPrxCallback> PropertyPlushFPrxCallbackPtr;

    /* callback of promise async proxy for client */
    class PropertyPlushFPrxCallbackPromise: public taf::ServantProxyCallback
    {
    public:
        virtual ~PropertyPlushFPrxCallbackPromise(){}
    public:
        struct Promiselogstat: virtual public TC_HandleBase
        {
        public:
            taf::Int32 _ret;
            map<std::string, std::string> _mRspContext;
        };
        
        typedef taf::TC_AutoPtr< PropertyPlushFPrxCallbackPromise::Promiselogstat > PromiselogstatPtr;

        PropertyPlushFPrxCallbackPromise(const promise::Promise< PropertyPlushFPrxCallbackPromise::PromiselogstatPtr > &promise)
        : _promise_logstat(promise)
        {}
        
        virtual void callback_logstat(const PropertyPlushFPrxCallbackPromise::PromiselogstatPtr &ptr)
        {
            _promise_logstat.setValue(ptr);
        }
        virtual void callback_logstat_exception(taf::Int32 ret)
        {
            std::string str("");
            str += "Function:logstat_exception|Ret:";
            str += TC_Common::tostr(ret);
            _promise_logstat.setException(promise::copyException(str, ret));
        }

    protected:
        promise::Promise< PropertyPlushFPrxCallbackPromise::PromiselogstatPtr > _promise_logstat;

    public:
        struct Promisemutillogstat: virtual public TC_HandleBase
        {
        public:
            taf::Int32 _ret;
            map<std::string, std::string> _mRspContext;
        };
        
        typedef taf::TC_AutoPtr< PropertyPlushFPrxCallbackPromise::Promisemutillogstat > PromisemutillogstatPtr;

        PropertyPlushFPrxCallbackPromise(const promise::Promise< PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr > &promise)
        : _promise_mutillogstat(promise)
        {}
        
        virtual void callback_mutillogstat(const PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr &ptr)
        {
            _promise_mutillogstat.setValue(ptr);
        }
        virtual void callback_mutillogstat_exception(taf::Int32 ret)
        {
            std::string str("");
            str += "Function:mutillogstat_exception|Ret:";
            str += TC_Common::tostr(ret);
            _promise_mutillogstat.setException(promise::copyException(str, ret));
        }

    protected:
        promise::Promise< PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr > _promise_mutillogstat;

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    };
    typedef taf::TC_AutoPtr<PropertyPlushFPrxCallbackPromise> PropertyPlushFPrxCallbackPromisePtr;

    /* callback of coroutine async proxy for client */
    class PropertyPlushFCoroPrxCallback: public PropertyPlushFPrxCallback
    {
    public:
        virtual ~PropertyPlushFCoroPrxCallback(){}
    public:
        virtual const map<std::string, std::string> & getResponseContext() const { return _mRspContext; }

        virtual void setResponseContext(const map<std::string, std::string> &mContext) { _mRspContext = mContext; }

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    protected:
        map<std::string, std::string> _mRspContext;
    };
    typedef taf::TC_AutoPtr<PropertyPlushFCoroPrxCallback> PropertyPlushFCoroPrxCallbackPtr;

    /* proxy for client */
    class PropertyPlushFProxy : public taf::ServantProxy
    {
    public:
        typedef map<string, string> TAF_CONTEXT;
        taf::Int32 logstat(const LogStat::StatLog & log,const map<string, string> &context = TAF_CONTEXT(),map<string, string> * pResponseContext = NULL);
        void async_logstat(PropertyPlushFPrxCallbackPtr callback,const LogStat::StatLog & log,const map<string, string> &context = TAF_CONTEXT());

        promise::Future< PropertyPlushFPrxCallbackPromise::PromiselogstatPtr > promise_async_logstat(const LogStat::StatLog & log,const map<string, string> &context = TAF_CONTEXT());
        void coro_logstat(PropertyPlushFCoroPrxCallbackPtr callback,const LogStat::StatLog & log,const map<string, string> &context = TAF_CONTEXT());

        taf::Int32 mutillogstat(const vector<LogStat::StatLog> & logs,const map<string, string> &context = TAF_CONTEXT(),map<string, string> * pResponseContext = NULL);
        void async_mutillogstat(PropertyPlushFPrxCallbackPtr callback,const vector<LogStat::StatLog> & logs,const map<string, string> &context = TAF_CONTEXT());

        promise::Future< PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr > promise_async_mutillogstat(const vector<LogStat::StatLog> & logs,const map<string, string> &context = TAF_CONTEXT());
        void coro_mutillogstat(PropertyPlushFCoroPrxCallbackPtr callback,const vector<LogStat::StatLog> & logs,const map<string, string> &context = TAF_CONTEXT());

        PropertyPlushFProxy* taf_hash(int64_t key);
        PropertyPlushFProxy* taf_consistent_hash(int64_t key);
        PropertyPlushFProxy* taf_set_timeout(int msecond);
        PropertyPlushFProxy* taf_loaded();
    };
    typedef taf::TC_AutoPtr<PropertyPlushFProxy> PropertyPlushFPrx;

    /* servant for server */
    class PropertyPlushF : public taf::Servant
    {
    public:
        virtual ~PropertyPlushF(){}
        virtual taf::Int32 logstat(const LogStat::StatLog & log,taf::JceCurrentPtr current) = 0;
        static void async_response_logstat(taf::JceCurrentPtr current, taf::Int32 _ret);

        virtual taf::Int32 mutillogstat(const vector<LogStat::StatLog> & logs,taf::JceCurrentPtr current) = 0;
        static void async_response_mutillogstat(taf::JceCurrentPtr current, taf::Int32 _ret);

    public:
        int onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer);
    };

    inline taf::Int32 PropertyPlushFProxy::logstat(const LogStat::StatLog &log, const map<string, string> &context,map<string, string> * pResponseContext)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(log, 1);
        taf::ResponsePacket rep;
        std::map<string, string> _mStatus;
        taf_invoke(taf::JCENORMAL,"logstat", _os.getByteBuffer(), context, _mStatus, rep);
        if(pResponseContext)
        {
            *pResponseContext = rep.context;
        }

        taf::JceInputStream<taf::BufferReader> _is;
        _is.setBuffer(rep.sBuffer);
        taf::Int32 _ret;
        _is.read(_ret, 0, true);
        return _ret;
    }

    inline void PropertyPlushF::async_response_logstat(taf::JceCurrentPtr current, taf::Int32 _ret)
    {
        if (current->getRequestVersion() == WUPVERSION || current->getRequestVersion() == WUPVERSION2)
        {
            UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
            tafAttr.setVersion(current->getRequestVersion());
            tafAttr.put("", _ret);

            vector<char> sWupResponseBuffer;
            tafAttr.encode(sWupResponseBuffer);
            current->sendResponse(taf::JCESERVERSUCCESS, sWupResponseBuffer);
        }
        else
        {
            taf::JceOutputStream<taf::BufferWriter> _os;
            _os.write(_ret, 0);

            current->sendResponse(taf::JCESERVERSUCCESS, _os.getByteBuffer());
        }
    }

    inline void PropertyPlushFProxy::async_logstat(PropertyPlushFPrxCallbackPtr callback,const LogStat::StatLog &log,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(log, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"logstat", _os.getByteBuffer(), context, _mStatus, callback);
    }
    
    inline promise::Future< PropertyPlushFPrxCallbackPromise::PromiselogstatPtr > PropertyPlushFProxy::promise_async_logstat(const LogStat::StatLog &log,const map<string, string>& context)
    {
        promise::Promise< PropertyPlushFPrxCallbackPromise::PromiselogstatPtr > promise;
        PropertyPlushFPrxCallbackPromisePtr callback = new PropertyPlushFPrxCallbackPromise(promise);

        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(log, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"logstat", _os.getByteBuffer(), context, _mStatus, callback);

        return promise.getFuture();
    }

    inline void PropertyPlushFProxy::coro_logstat(PropertyPlushFCoroPrxCallbackPtr callback,const LogStat::StatLog &log,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(log, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"logstat", _os.getByteBuffer(), context, _mStatus, callback, true);
    }

    inline taf::Int32 PropertyPlushFProxy::mutillogstat(const vector<LogStat::StatLog> &logs, const map<string, string> &context,map<string, string> * pResponseContext)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(logs, 1);
        taf::ResponsePacket rep;
        std::map<string, string> _mStatus;
        taf_invoke(taf::JCENORMAL,"mutillogstat", _os.getByteBuffer(), context, _mStatus, rep);
        if(pResponseContext)
        {
            *pResponseContext = rep.context;
        }

        taf::JceInputStream<taf::BufferReader> _is;
        _is.setBuffer(rep.sBuffer);
        taf::Int32 _ret;
        _is.read(_ret, 0, true);
        return _ret;
    }

    inline void PropertyPlushF::async_response_mutillogstat(taf::JceCurrentPtr current, taf::Int32 _ret)
    {
        if (current->getRequestVersion() == WUPVERSION || current->getRequestVersion() == WUPVERSION2)
        {
            UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
            tafAttr.setVersion(current->getRequestVersion());
            tafAttr.put("", _ret);

            vector<char> sWupResponseBuffer;
            tafAttr.encode(sWupResponseBuffer);
            current->sendResponse(taf::JCESERVERSUCCESS, sWupResponseBuffer);
        }
        else
        {
            taf::JceOutputStream<taf::BufferWriter> _os;
            _os.write(_ret, 0);

            current->sendResponse(taf::JCESERVERSUCCESS, _os.getByteBuffer());
        }
    }

    inline void PropertyPlushFProxy::async_mutillogstat(PropertyPlushFPrxCallbackPtr callback,const vector<LogStat::StatLog> &logs,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(logs, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"mutillogstat", _os.getByteBuffer(), context, _mStatus, callback);
    }
    
    inline promise::Future< PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr > PropertyPlushFProxy::promise_async_mutillogstat(const vector<LogStat::StatLog> &logs,const map<string, string>& context)
    {
        promise::Promise< PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr > promise;
        PropertyPlushFPrxCallbackPromisePtr callback = new PropertyPlushFPrxCallbackPromise(promise);

        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(logs, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"mutillogstat", _os.getByteBuffer(), context, _mStatus, callback);

        return promise.getFuture();
    }

    inline void PropertyPlushFProxy::coro_mutillogstat(PropertyPlushFCoroPrxCallbackPtr callback,const vector<LogStat::StatLog> &logs,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(logs, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"mutillogstat", _os.getByteBuffer(), context, _mStatus, callback, true);
    }

    inline PropertyPlushFProxy* PropertyPlushFProxy::taf_hash(int64_t key)
    {
        return (PropertyPlushFProxy*)ServantProxy::taf_hash(key);
    }

    inline PropertyPlushFProxy* PropertyPlushFProxy::taf_consistent_hash(int64_t key)
    {
        return (PropertyPlushFProxy*)ServantProxy::taf_consistent_hash(key);
    }

    inline PropertyPlushFProxy* PropertyPlushFProxy::taf_set_timeout(int msecond)
    {
        return (PropertyPlushFProxy*)ServantProxy::taf_set_timeout(msecond);
    }

    inline PropertyPlushFProxy* PropertyPlushFProxy::taf_loaded()
    {
        return (PropertyPlushFProxy*)ServantProxy::taf_loaded();
    }

    static ::std::string __LogStat__PropertyPlushF_all[]=
    {
        "logstat",
        "mutillogstat"
    };

    inline const map<std::string, std::string> & PropertyPlushFPrxCallback::getResponseContext() const
    {
        CallbackThreadData * pCbtd = CallbackThreadData::getData();
        assert(pCbtd != NULL);

        if(!pCbtd->getContextValid())
        {
            throw TC_Exception("cann't get response context");
        }
        return pCbtd->getResponseContext();
    }

    inline int PropertyPlushFPrxCallback::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__LogStat__PropertyPlushF_all, __LogStat__PropertyPlushF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __LogStat__PropertyPlushF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_logstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                taf::Int32 _ret;
                _is.read(_ret, 0, true);

                CallbackThreadData * pCbtd = CallbackThreadData::getData();
                assert(pCbtd != NULL);

                pCbtd->setResponseContext(msg->response.context);

                callback_logstat(_ret);

                pCbtd->delResponseContext();

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_mutillogstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                taf::Int32 _ret;
                _is.read(_ret, 0, true);

                CallbackThreadData * pCbtd = CallbackThreadData::getData();
                assert(pCbtd != NULL);

                pCbtd->setResponseContext(msg->response.context);

                callback_mutillogstat(_ret);

                pCbtd->delResponseContext();

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int PropertyPlushFPrxCallbackPromise::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__LogStat__PropertyPlushF_all, __LogStat__PropertyPlushF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __LogStat__PropertyPlushF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_logstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);

                PropertyPlushFPrxCallbackPromise::PromiselogstatPtr ptr = new PropertyPlushFPrxCallbackPromise::Promiselogstat();

                _is.read(ptr->_ret, 0, true);


                ptr->_mRspContext = msg->response.context;

                callback_logstat(ptr);

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_mutillogstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);

                PropertyPlushFPrxCallbackPromise::PromisemutillogstatPtr ptr = new PropertyPlushFPrxCallbackPromise::Promisemutillogstat();

                _is.read(ptr->_ret, 0, true);


                ptr->_mRspContext = msg->response.context;

                callback_mutillogstat(ptr);

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int PropertyPlushFCoroPrxCallback::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__LogStat__PropertyPlushF_all, __LogStat__PropertyPlushF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __LogStat__PropertyPlushF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_logstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                taf::Int32 _ret;
                _is.read(_ret, 0, true);

                setResponseContext(msg->response.context);

                callback_logstat(_ret);

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_mutillogstat_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                taf::Int32 _ret;
                _is.read(_ret, 0, true);

                setResponseContext(msg->response.context);

                callback_mutillogstat(_ret);

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int PropertyPlushF::onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer)
    {
        pair<string*, string*> r = equal_range(__LogStat__PropertyPlushF_all, __LogStat__PropertyPlushF_all+2, _current->getFuncName());
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __LogStat__PropertyPlushF_all)
        {
            case 0:
            {
                taf::JceInputStream<taf::BufferReader> _is;
                _is.setBuffer(_current->getRequestBuffer());
                LogStat::StatLog log;
                if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
                    UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                    tafAttr.setVersion(_current->getRequestVersion());
                    tafAttr.decode(_current->getRequestBuffer());
                    tafAttr.get("log", log);
                }
                else
                {
                    _is.read(log, 1, true);
                }
                taf::Int32 _ret = logstat(log, _current);
                if(_current->isResponse())
                {
                    if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                    {
                        UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                        tafAttr.setVersion(_current->getRequestVersion());
                        tafAttr.put("", _ret);
                        tafAttr.encode(_sResponseBuffer);
                    }
                    else
                    {
                        taf::JceOutputStream<taf::BufferWriter> _os;
                        _os.write(_ret, 0);
                        _os.swap(_sResponseBuffer);
                    }
                }
                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                taf::JceInputStream<taf::BufferReader> _is;
                _is.setBuffer(_current->getRequestBuffer());
                vector<LogStat::StatLog> logs;
                if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
                    UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                    tafAttr.setVersion(_current->getRequestVersion());
                    tafAttr.decode(_current->getRequestBuffer());
                    tafAttr.get("logs", logs);
                }
                else
                {
                    _is.read(logs, 1, true);
                }
                taf::Int32 _ret = mutillogstat(logs, _current);
                if(_current->isResponse())
                {
                    if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                    {
                        UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                        tafAttr.setVersion(_current->getRequestVersion());
                        tafAttr.put("", _ret);
                        tafAttr.encode(_sResponseBuffer);
                    }
                    else
                    {
                        taf::JceOutputStream<taf::BufferWriter> _os;
                        _os.write(_ret, 0);
                        _os.swap(_sResponseBuffer);
                    }
                }
                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

} // end namespace LogStat

#define LogStat_StatLog_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.logname,b.logname);jce_copy_struct(a.content,b.content);jce_copy_struct(a.date,b.date);jce_copy_struct(a.flag,b.flag);jce_copy_struct(a.extend,b.extend);

#define LogStat_StatValue_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.policy,b.policy);jce_copy_struct(a.value,b.value);jce_copy_struct(a.count,b.count);

#define LogStat_StatContent_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.keys,b.keys);jce_copy_struct(a.values,b.values);

#define LogStat_StatOneLog_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.logname,b.logname);jce_copy_struct(a.keys,b.keys);jce_copy_struct(a.values,b.values);


#endif

