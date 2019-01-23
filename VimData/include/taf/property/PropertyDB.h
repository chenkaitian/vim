// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.2.2.11 by WSRD Tencent.
// Generated from `PropertyDB.jce'
// **********************************************************************

#ifndef __PROPERTYDB_H_
#define __PROPERTYDB_H_

#include <map>
#include <string>
#include <vector>
#include "jce/Jce.h"
#include "jce/wup.h"
#include "servant/BaseF.h"

using namespace wup;
using namespace std;



namespace taf
{
    struct Db : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "taf.Db";
        }
        static string MD5()
        {
            return "8aef3711ca93f682d5e01ca071c1d888";
        }
        Db()
        :sModule(""),sIp(""),sDbName(""),sTbName(""),sUser(""),sPass(""),sPort("")
        {
        }
        void resetDefautlt()
        {
            sModule = "";
            sIp = "";
            sDbName = "";
            sTbName = "";
            sUser = "";
            sPass = "";
            sPort = "";
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(sModule, 0);
            _os.write(sIp, 1);
            _os.write(sDbName, 2);
            _os.write(sTbName, 3);
            _os.write(sUser, 4);
            _os.write(sPass, 5);
            _os.write(sPort, 6);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(sModule, 0, true);
            _is.read(sIp, 1, true);
            _is.read(sDbName, 2, true);
            _is.read(sTbName, 3, true);
            _is.read(sUser, 4, true);
            _is.read(sPass, 5, true);
            _is.read(sPort, 6, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(sModule,"sModule");
            _ds.display(sIp,"sIp");
            _ds.display(sDbName,"sDbName");
            _ds.display(sTbName,"sTbName");
            _ds.display(sUser,"sUser");
            _ds.display(sPass,"sPass");
            _ds.display(sPort,"sPort");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(sModule, true);
            _ds.displaySimple(sIp, true);
            _ds.displaySimple(sDbName, true);
            _ds.displaySimple(sTbName, true);
            _ds.displaySimple(sUser, true);
            _ds.displaySimple(sPass, true);
            _ds.displaySimple(sPort, false);
            return _os;
        }
    public:
        std::string sModule;
        std::string sIp;
        std::string sDbName;
        std::string sTbName;
        std::string sUser;
        std::string sPass;
        std::string sPort;
    };
    inline bool operator==(const Db&l, const Db&r)
    {
        return l.sModule == r.sModule && l.sIp == r.sIp && l.sDbName == r.sDbName && l.sTbName == r.sTbName && l.sUser == r.sUser && l.sPass == r.sPass && l.sPort == r.sPort;
    }
    inline bool operator!=(const Db&l, const Db&r)
    {
        return !(l == r);
    }


} // end namespace taf

#define taf_Db_JCE_COPY_STRUCT_HELPER   \
        jce_copy_struct(a.sModule,b.sModule);jce_copy_struct(a.sIp,b.sIp);jce_copy_struct(a.sDbName,b.sDbName);jce_copy_struct(a.sTbName,b.sTbName);jce_copy_struct(a.sUser,b.sUser);jce_copy_struct(a.sPass,b.sPass);jce_copy_struct(a.sPort,b.sPort);


#endif
