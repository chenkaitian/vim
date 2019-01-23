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
����ָ����๤��
1 ����ָ����඼��ͨ����������
2 Ϊ�˱�֤��Ч��,����ָ���඼ֻ����һ��,����������
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
����ָ�������
1 ���д���ָ��Ķ��̳�������
2 �������඼���Զ�̬�ļ���,�����޸Ĵ���,�����±��뼴��
3 P_Command�౾���ǲ�Ӧ�����ɵ�,����϶��������ļ���û����ȷ����
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
