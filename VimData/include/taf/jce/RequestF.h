// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.0.0.26 by WSRD Tencent.
// Generated from `RequestF.jce'
// **********************************************************************

#ifndef __REQUESTF_H_
#define __REQUESTF_H_

#include <map>
#include <string>
#include <vector>
#include "jce/Jce.h"
using namespace std;


namespace taf
{
    struct RequestPacket : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "taf.RequestPacket";
        }
        static string MD5()
        {
            return "ca2347e333175043f3ec17083b16e36d";
        }
        RequestPacket()
        :iVersion(0),cPacketType(0),iMessageType(0),iRequestId(0),sServantName(""),sFuncName(""),iTimeout(0)
        {
        }
        void resetDefautlt()
        {
            iVersion = 0;
            cPacketType = 0;
            iMessageType = 0;
            iRequestId = 0;
            sServantName = "";
            sFuncName = "";
            iTimeout = 0;
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(iVersion, 1);
            _os.write(cPacketType, 2);
            _os.write(iMessageType, 3);
            _os.write(iRequestId, 4);
            _os.write(sServantName, 5);
            _os.write(sFuncName, 6);
            _os.write(sBuffer, 7);
            _os.write(iTimeout, 8);
            _os.write(context, 9);
            _os.write(status, 10);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(iVersion, 1, true);
            _is.read(cPacketType, 2, true);
            _is.read(iMessageType, 3, true);
            _is.read(iRequestId, 4, true);
            _is.read(sServantName, 5, true);
            _is.read(sFuncName, 6, true);
            _is.read(sBuffer, 7, true);
            _is.read(iTimeout, 8, true);
            _is.read(context, 9, true);
            _is.read(status, 10, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(iVersion,"iVersion");
            _ds.display(cPacketType,"cPacketType");
            _ds.display(iMessageType,"iMessageType");
            _ds.display(iRequestId,"iRequestId");
            _ds.display(sServantName,"sServantName");
            _ds.display(sFuncName,"sFuncName");
            _ds.display(sBuffer,"sBuffer");
            _ds.display(iTimeout,"iTimeout");
            _ds.display(context,"context");
            _ds.display(status,"status");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(iVersion, true);
            _ds.displaySimple(cPacketType, true);
            _ds.displaySimple(iMessageType, true);
            _ds.displaySimple(iRequestId, true);
            _ds.displaySimple(sServantName, true);
            _ds.displaySimple(sFuncName, true);
            _ds.displaySimple(sBuffer, true);
            _ds.displaySimple(iTimeout, true);
            _ds.displaySimple(context, true);
            _ds.displaySimple(status, false);
            return _os;
        }
    public:
        taf::Short iVersion;
        taf::Char cPacketType;
        taf::Int32 iMessageType;
        taf::Int32 iRequestId;
        std::string sServantName;
        std::string sFuncName;
        vector<taf::Char> sBuffer;
        taf::Int32 iTimeout;
        map<std::string, std::string> context;
        map<std::string, std::string> status;
    };
    inline bool operator==(const RequestPacket&l, const RequestPacket&r)
    {
        return l.iVersion == r.iVersion && l.cPacketType == r.cPacketType && l.iMessageType == r.iMessageType && l.iRequestId == r.iRequestId && l.sServantName == r.sServantName && l.sFuncName == r.sFuncName && l.sBuffer == r.sBuffer && l.iTimeout == r.iTimeout && l.context == r.context && l.status == r.status;
    }
    inline bool operator!=(const RequestPacket&l, const RequestPacket&r)
    {
        return !(l == r);
    }

    struct ResponsePacket : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "taf.ResponsePacket";
        }
        static string MD5()
        {
            return "9a587534dfcf0c56e30b40d2493d8da7";
        }
        ResponsePacket()
        :iVersion(0),cPacketType(0),iRequestId(0),iMessageType(0),iRet(0),sResultDesc("")
        {
        }
        void resetDefautlt()
        {
            iVersion = 0;
            cPacketType = 0;
            iRequestId = 0;
            iMessageType = 0;
            iRet = 0;
            sResultDesc = "";
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(iVersion, 1);
            _os.write(cPacketType, 2);
            _os.write(iRequestId, 3);
            _os.write(iMessageType, 4);
            _os.write(iRet, 5);
            _os.write(sBuffer, 6);
            _os.write(status, 7);
            _os.write(sResultDesc, 8);
            _os.write(context, 9);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(iVersion, 1, true);
            _is.read(cPacketType, 2, true);
            _is.read(iRequestId, 3, true);
            _is.read(iMessageType, 4, true);
            _is.read(iRet, 5, true);
            _is.read(sBuffer, 6, true);
            _is.read(status, 7, true);
            _is.read(sResultDesc, 8, false);
            _is.read(context, 9, false);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(iVersion,"iVersion");
            _ds.display(cPacketType,"cPacketType");
            _ds.display(iRequestId,"iRequestId");
            _ds.display(iMessageType,"iMessageType");
            _ds.display(iRet,"iRet");
            _ds.display(sBuffer,"sBuffer");
            _ds.display(status,"status");
            _ds.display(sResultDesc,"sResultDesc");
            _ds.display(context,"context");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(iVersion, true);
            _ds.displaySimple(cPacketType, true);
            _ds.displaySimple(iRequestId, true);
            _ds.displaySimple(iMessageType, true);
            _ds.displaySimple(iRet, true);
            _ds.displaySimple(sBuffer, true);
            _ds.displaySimple(status, true);
            _ds.displaySimple(sResultDesc, true);
            _ds.displaySimple(context, false);
            return _os;
        }
    public:
        taf::Short iVersion;
        taf::Char cPacketType;
        taf::Int32 iRequestId;
        taf::Int32 iMessageType;
        taf::Int32 iRet;
        vector<taf::Char> sBuffer;
        map<std::string, std::string> status;
        std::string sResultDesc;
        map<std::string, std::string> context;
    };
    inline bool operator==(const ResponsePacket&l, const ResponsePacket&r)
    {
        return l.iVersion == r.iVersion && l.cPacketType == r.cPacketType && l.iRequestId == r.iRequestId && l.iMessageType == r.iMessageType && l.iRet == r.iRet && l.sBuffer == r.sBuffer && l.status == r.status && l.sResultDesc == r.sResultDesc && l.context == r.context;
    }
    inline bool operator!=(const ResponsePacket&l, const ResponsePacket&r)
    {
        return !(l == r);
    }


}

#define taf_RequestPacket_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.iVersion,b.iVersion);jce_copy_struct(a.cPacketType,b.cPacketType);jce_copy_struct(a.iMessageType,b.iMessageType);jce_copy_struct(a.iRequestId,b.iRequestId);jce_copy_struct(a.sServantName,b.sServantName);jce_copy_struct(a.sFuncName,b.sFuncName);jce_copy_struct(a.sBuffer,b.sBuffer);jce_copy_struct(a.iTimeout,b.iTimeout);jce_copy_struct(a.context,b.context);jce_copy_struct(a.status,b.status);

#define taf_ResponsePacket_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.iVersion,b.iVersion);jce_copy_struct(a.cPacketType,b.cPacketType);jce_copy_struct(a.iRequestId,b.iRequestId);jce_copy_struct(a.iMessageType,b.iMessageType);jce_copy_struct(a.iRet,b.iRet);jce_copy_struct(a.sBuffer,b.sBuffer);jce_copy_struct(a.status,b.status);jce_copy_struct(a.sResultDesc,b.sResultDesc);jce_copy_struct(a.context,b.context);



#endif
