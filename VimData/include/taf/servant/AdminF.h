// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.2.2.11 by WSRD Tencent.
// Generated from `AdminF.jce'
// **********************************************************************

#ifndef __ADMINF_H_
#define __ADMINF_H_

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


namespace taf
{
    /* callback of async proxy for client */
    class AdminFPrxCallback: public taf::ServantProxyCallback
    {
    public:
        virtual ~AdminFPrxCallback(){}
        virtual void callback_shutdown()
        { throw std::runtime_error("callback_shutdown() override incorrect."); }
        virtual void callback_shutdown_exception(taf::Int32 ret)
        { throw std::runtime_error("callback_shutdown_exception() override incorrect."); }

        virtual void callback_notify(const std::string& ret)
        { throw std::runtime_error("callback_notify() override incorrect."); }
        virtual void callback_notify_exception(taf::Int32 ret)
        { throw std::runtime_error("callback_notify_exception() override incorrect."); }

    public:
        virtual const map<std::string, std::string> & getResponseContext() const;

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    };
    typedef taf::TC_AutoPtr<AdminFPrxCallback> AdminFPrxCallbackPtr;

    /* callback of promise async proxy for client */
    class AdminFPrxCallbackPromise: public taf::ServantProxyCallback
    {
    public:
        virtual ~AdminFPrxCallbackPromise(){}
    public:
        struct Promiseshutdown: virtual public TC_HandleBase
        {
        public:
            map<std::string, std::string> _mRspContext;
        };
        
        typedef taf::TC_AutoPtr< AdminFPrxCallbackPromise::Promiseshutdown > PromiseshutdownPtr;

        AdminFPrxCallbackPromise(const promise::Promise< AdminFPrxCallbackPromise::PromiseshutdownPtr > &promise)
        : _promise_shutdown(promise)
        {}
        
        virtual void callback_shutdown(const AdminFPrxCallbackPromise::PromiseshutdownPtr &ptr)
        {
            _promise_shutdown.setValue(ptr);
        }
        virtual void callback_shutdown_exception(taf::Int32 ret)
        {
            std::string str("");
            str += "Function:shutdown_exception|Ret:";
            str += TC_Common::tostr(ret);
            _promise_shutdown.setException(promise::copyException(str, ret));
        }

    protected:
        promise::Promise< AdminFPrxCallbackPromise::PromiseshutdownPtr > _promise_shutdown;

    public:
        struct Promisenotify: virtual public TC_HandleBase
        {
        public:
            std::string _ret;
            map<std::string, std::string> _mRspContext;
        };
        
        typedef taf::TC_AutoPtr< AdminFPrxCallbackPromise::Promisenotify > PromisenotifyPtr;

        AdminFPrxCallbackPromise(const promise::Promise< AdminFPrxCallbackPromise::PromisenotifyPtr > &promise)
        : _promise_notify(promise)
        {}
        
        virtual void callback_notify(const AdminFPrxCallbackPromise::PromisenotifyPtr &ptr)
        {
            _promise_notify.setValue(ptr);
        }
        virtual void callback_notify_exception(taf::Int32 ret)
        {
            std::string str("");
            str += "Function:notify_exception|Ret:";
            str += TC_Common::tostr(ret);
            _promise_notify.setException(promise::copyException(str, ret));
        }

    protected:
        promise::Promise< AdminFPrxCallbackPromise::PromisenotifyPtr > _promise_notify;

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    };
    typedef taf::TC_AutoPtr<AdminFPrxCallbackPromise> AdminFPrxCallbackPromisePtr;

    /* callback of coroutine async proxy for client */
    class AdminFCoroPrxCallback: public AdminFPrxCallback
    {
    public:
        virtual ~AdminFCoroPrxCallback(){}
    public:
        virtual const map<std::string, std::string> & getResponseContext() const { return _mRspContext; }

        virtual void setResponseContext(const map<std::string, std::string> &mContext) { _mRspContext = mContext; }

    public:
        virtual int onDispatch(taf::ReqMessagePtr msg);
    protected:
        map<std::string, std::string> _mRspContext;
    };
    typedef taf::TC_AutoPtr<AdminFCoroPrxCallback> AdminFCoroPrxCallbackPtr;

    /* proxy for client */
    class AdminFProxy : public taf::ServantProxy
    {
    public:
        typedef map<string, string> TAF_CONTEXT;
        void shutdown(const map<string, string> &context = TAF_CONTEXT(),map<string, string> * pResponseContext = NULL);
        void async_shutdown(AdminFPrxCallbackPtr callback,const map<string, string> &context = TAF_CONTEXT());

        promise::Future< AdminFPrxCallbackPromise::PromiseshutdownPtr > promise_async_shutdown(const map<string, string> &context = TAF_CONTEXT());
        void coro_shutdown(AdminFCoroPrxCallbackPtr callback,const map<string, string> &context = TAF_CONTEXT());

        std::string notify(const std::string & command,const map<string, string> &context = TAF_CONTEXT(),map<string, string> * pResponseContext = NULL);
        void async_notify(AdminFPrxCallbackPtr callback,const std::string & command,const map<string, string> &context = TAF_CONTEXT());

        promise::Future< AdminFPrxCallbackPromise::PromisenotifyPtr > promise_async_notify(const std::string & command,const map<string, string> &context = TAF_CONTEXT());
        void coro_notify(AdminFCoroPrxCallbackPtr callback,const std::string & command,const map<string, string> &context = TAF_CONTEXT());

        AdminFProxy* taf_hash(int64_t key);
        AdminFProxy* taf_consistent_hash(int64_t key);
        AdminFProxy* taf_set_timeout(int msecond);
        AdminFProxy* taf_loaded();
    };
    typedef taf::TC_AutoPtr<AdminFProxy> AdminFPrx;

    /* servant for server */
    class AdminF : public taf::Servant
    {
    public:
        virtual ~AdminF(){}
        virtual void shutdown(taf::JceCurrentPtr current) = 0;
        static void async_response_shutdown(taf::JceCurrentPtr current);

        virtual std::string notify(const std::string & command,taf::JceCurrentPtr current) = 0;
        static void async_response_notify(taf::JceCurrentPtr current, const std::string &_ret);

    public:
        int onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer);
    };

    inline std::string AdminFProxy::notify(const std::string &command, const map<string, string> &context,map<string, string> * pResponseContext)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(command, 1);
        taf::ResponsePacket rep;
        std::map<string, string> _mStatus;
        taf_invoke(taf::JCENORMAL,"notify", _os.getByteBuffer(), context, _mStatus, rep);
        if(pResponseContext)
        {
            *pResponseContext = rep.context;
        }

        taf::JceInputStream<taf::BufferReader> _is;
        _is.setBuffer(rep.sBuffer);
        std::string _ret;
        _is.read(_ret, 0, true);
        return _ret;
    }

    inline void AdminF::async_response_notify(taf::JceCurrentPtr current, const std::string &_ret)
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

    inline void AdminFProxy::async_notify(AdminFPrxCallbackPtr callback,const std::string &command,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(command, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"notify", _os.getByteBuffer(), context, _mStatus, callback);
    }
    
    inline promise::Future< AdminFPrxCallbackPromise::PromisenotifyPtr > AdminFProxy::promise_async_notify(const std::string &command,const map<string, string>& context)
    {
        promise::Promise< AdminFPrxCallbackPromise::PromisenotifyPtr > promise;
        AdminFPrxCallbackPromisePtr callback = new AdminFPrxCallbackPromise(promise);

        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(command, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"notify", _os.getByteBuffer(), context, _mStatus, callback);

        return promise.getFuture();
    }

    inline void AdminFProxy::coro_notify(AdminFCoroPrxCallbackPtr callback,const std::string &command,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        _os.write(command, 1);
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"notify", _os.getByteBuffer(), context, _mStatus, callback, true);
    }

    inline void AdminFProxy::shutdown(const map<string, string> &context,map<string, string> * pResponseContext)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        taf::ResponsePacket rep;
        std::map<string, string> _mStatus;
        taf_invoke(taf::JCENORMAL,"shutdown", _os.getByteBuffer(), context, _mStatus, rep);
        if(pResponseContext)
        {
            *pResponseContext = rep.context;
        }

    }

    inline void AdminF::async_response_shutdown(taf::JceCurrentPtr current)
    {
        if (current->getRequestVersion() == WUPVERSION || current->getRequestVersion() == WUPVERSION2)
        {
            UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
            tafAttr.setVersion(current->getRequestVersion());

            vector<char> sWupResponseBuffer;
            tafAttr.encode(sWupResponseBuffer);
            current->sendResponse(taf::JCESERVERSUCCESS, sWupResponseBuffer);
        }
        else
        {
            taf::JceOutputStream<taf::BufferWriter> _os;
            current->sendResponse(taf::JCESERVERSUCCESS, _os.getByteBuffer());
        }
    }

    inline void AdminFProxy::async_shutdown(AdminFPrxCallbackPtr callback,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"shutdown", _os.getByteBuffer(), context, _mStatus, callback);
    }
    
    inline promise::Future< AdminFPrxCallbackPromise::PromiseshutdownPtr > AdminFProxy::promise_async_shutdown(const map<string, string>& context)
    {
        promise::Promise< AdminFPrxCallbackPromise::PromiseshutdownPtr > promise;
        AdminFPrxCallbackPromisePtr callback = new AdminFPrxCallbackPromise(promise);

        taf::JceOutputStream<taf::BufferWriter> _os;
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"shutdown", _os.getByteBuffer(), context, _mStatus, callback);

        return promise.getFuture();
    }

    inline void AdminFProxy::coro_shutdown(AdminFCoroPrxCallbackPtr callback,const map<string, string>& context)
    {
        taf::JceOutputStream<taf::BufferWriter> _os;
        std::map<string, string> _mStatus;
        taf_invoke_async(taf::JCENORMAL,"shutdown", _os.getByteBuffer(), context, _mStatus, callback, true);
    }

    inline AdminFProxy* AdminFProxy::taf_hash(int64_t key)
    {
        return (AdminFProxy*)ServantProxy::taf_hash(key);
    }

    inline AdminFProxy* AdminFProxy::taf_consistent_hash(int64_t key)
    {
        return (AdminFProxy*)ServantProxy::taf_consistent_hash(key);
    }

    inline AdminFProxy* AdminFProxy::taf_set_timeout(int msecond)
    {
        return (AdminFProxy*)ServantProxy::taf_set_timeout(msecond);
    }

    inline AdminFProxy* AdminFProxy::taf_loaded()
    {
        return (AdminFProxy*)ServantProxy::taf_loaded();
    }

    static ::std::string __taf__AdminF_all[]=
    {
        "notify",
        "shutdown"
    };

    inline const map<std::string, std::string> & AdminFPrxCallback::getResponseContext() const
    {
        CallbackThreadData * pCbtd = CallbackThreadData::getData();
        assert(pCbtd != NULL);

        if(!pCbtd->getContextValid())
        {
            throw TC_Exception("cann't get response context");
        }
        return pCbtd->getResponseContext();
    }

    inline int AdminFPrxCallback::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__taf__AdminF_all, __taf__AdminF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __taf__AdminF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_notify_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                std::string _ret;
                _is.read(_ret, 0, true);

                CallbackThreadData * pCbtd = CallbackThreadData::getData();
                assert(pCbtd != NULL);

                pCbtd->setResponseContext(msg->response.context);

                callback_notify(_ret);

                pCbtd->delResponseContext();

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_shutdown_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                CallbackThreadData * pCbtd = CallbackThreadData::getData();
                assert(pCbtd != NULL);

                pCbtd->setResponseContext(msg->response.context);

                callback_shutdown();

                pCbtd->delResponseContext();

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int AdminFPrxCallbackPromise::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__taf__AdminF_all, __taf__AdminF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __taf__AdminF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_notify_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);

                AdminFPrxCallbackPromise::PromisenotifyPtr ptr = new AdminFPrxCallbackPromise::Promisenotify();

                _is.read(ptr->_ret, 0, true);


                ptr->_mRspContext = msg->response.context;

                callback_notify(ptr);

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_shutdown_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);

                AdminFPrxCallbackPromise::PromiseshutdownPtr ptr = new AdminFPrxCallbackPromise::Promiseshutdown();


                ptr->_mRspContext = msg->response.context;

                callback_shutdown(ptr);

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int AdminFCoroPrxCallback::onDispatch(taf::ReqMessagePtr msg)
    {
        pair<string*, string*> r = equal_range(__taf__AdminF_all, __taf__AdminF_all+2, string(msg->request.sFuncName));
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __taf__AdminF_all)
        {
            case 0:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_notify_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                std::string _ret;
                _is.read(_ret, 0, true);

                setResponseContext(msg->response.context);

                callback_notify(_ret);

                return taf::JCESERVERSUCCESS;

            }
            case 1:
            {
                if (msg->response.iRet != taf::JCESERVERSUCCESS)
                {
                    callback_shutdown_exception(msg->response.iRet);

                    return msg->response.iRet;
                }
                taf::JceInputStream<taf::BufferReader> _is;

                _is.setBuffer(msg->response.sBuffer);
                setResponseContext(msg->response.context);

                callback_shutdown();

                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

    inline int AdminF::onDispatch(taf::JceCurrentPtr _current, vector<char> &_sResponseBuffer)
    {
        pair<string*, string*> r = equal_range(__taf__AdminF_all, __taf__AdminF_all+2, _current->getFuncName());
        if(r.first == r.second) return taf::JCESERVERNOFUNCERR;
        switch(r.first - __taf__AdminF_all)
        {
            case 0:
            {
                taf::JceInputStream<taf::BufferReader> _is;
                _is.setBuffer(_current->getRequestBuffer());
                std::string command;
                if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
                    UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                    tafAttr.setVersion(_current->getRequestVersion());
                    tafAttr.decode(_current->getRequestBuffer());
                    tafAttr.get("command", command);
                }
                else
                {
                    _is.read(command, 1, true);
                }
                std::string _ret = notify(command, _current);
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
                if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                {
                    UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                    tafAttr.setVersion(_current->getRequestVersion());
                    tafAttr.decode(_current->getRequestBuffer());
                }
                else
                {
                }
                shutdown(_current);
                if(_current->isResponse())
                {
                    if (_current->getRequestVersion() == WUPVERSION || _current->getRequestVersion() == WUPVERSION2)
                    {
                        UniAttribute<taf::BufferWriter, taf::BufferReader>  tafAttr;
                        tafAttr.setVersion(_current->getRequestVersion());
                        tafAttr.encode(_sResponseBuffer);
                    }
                    else
                    {
                        taf::JceOutputStream<taf::BufferWriter> _os;
                        _os.swap(_sResponseBuffer);
                    }
                }
                return taf::JCESERVERSUCCESS;

            }
        }
        return taf::JCESERVERNOFUNCERR;
    }

} // end namespace taf


#endif

