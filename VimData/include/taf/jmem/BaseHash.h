#ifndef _BASEHASH_H
#define _BASEHASH_H
#include <string>
#include <map>

#include "util/tc_dyn_object.h"

using namespace std;
using namespace taf;

template<typename T>
class SVR_Globe
{
public:

    static T& GetInstance()
    {
        static T *pT = NULL;
        if(pT != NULL)
        {
            return (*pT);
        }

        pT = new T();

        return (*pT);
    }

protected:
    SVR_Globe(){};

};

class P_Hash;

/**
定义指令处理类工厂
1 所有指令处理类都是通过该类生成
2 为了保证高效性,所有指令类都只生成一次,并保存下来
*/
class P_Hash_Factory
{
public:
    ~P_Hash_Factory();

    P_Hash *operator[](const string &sHashName);

protected:
    map<string, P_Hash*>     m_mpHashs;
};

#define HASH_FACTORY SVR_Globe<P_Hash_Factory>::GetInstance()

/**
定义指令操作类
1 所有处理指令的都继承至该类
2 所有子类都可以动态的加载,无需修改代码,重新新编译即可
3 P_Command类本身是不应该生成的,否则肯定是配置文件中没有正确配置
*/
class P_Hash //: public TC_DYN_Object
{
public:
    //DECLARE_DYNCREATE(P_Hash)

    P_Hash(){};
    virtual ~P_Hash(){};

    virtual size_t HashRawInt(const int key){ return 0;}
	virtual size_t HashRawLong(const long long key){ return 0;}
	virtual size_t HashRawString(const string & key){ return 0;}
	virtual size_t HashInt(const string & key){ return 0;}
	virtual size_t HashLong(const string & key){ return 0;}
	virtual size_t HashString(const string & key){ return 0;}

};

#endif
