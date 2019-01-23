#ifndef _NORMALHASH_H
#define _NORMALHASH_H

#include "BaseHash.h"
//#include "CacheShare.h"

//using namespace DCache;

class NormalHash : public P_Hash
{
public:
    //DECLARE_DYNCREATE(NormalHash)

    NormalHash(){};
    virtual ~NormalHash(){};

    virtual size_t HashRawInt(const int key);
	virtual size_t HashRawLong(const long long key);
	virtual size_t HashRawString(const string &key);

	//virtual size_t HashInt(const string &key);
	//virtual size_t HashLong(const string & key);
	virtual size_t HashString(const string &key);
private:
	//void HashCombine(size_t& seed, const char v);

};

#endif
