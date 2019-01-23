#ifndef JMEM_POLICY_NEW_H
#define JMEM_POLICY_NEW_H

#include "util/tc_thread_mutex.h"
#include "sem_mutex_auto.h"
//#include "util/tc_shm.h"
#include "util/tc_mmap.h"
namespace taf
{
//////////////////////////////////////////////////////////////////////
// �洢����: �ڴ�, �����ڴ�, mmap(�ļ�)

/**
 * �ڴ�洢
 */
template<typename T, typename LockPolicy>
class MemStorePolicyAuto : public LockPolicy
{
public:
    /**
    * ��ʼ��
    * @param pAddr: ָ����пռ��ָ��
    * @param iSize: �ռ��ָ��
    */
    void create(void *pAddr, size_t iSize)
    {
        _t.create(pAddr,iSize);
    }

    /**
    * �����϶���
    * @param pAddr: ָ����пռ��ָ��
    * @param iSize: �ռ��ָ��
    */
    void connect(void *pAddr, size_t iSize)
    {
        _t.connect(pAddr,iSize);
    }

protected:
    T   _t;
};

/**
 * �����ڴ�洢
 */
template<typename T, typename LockPolicy>
class ShmStorePolicyAuto : public LockPolicy
{
public:
    /**
     * ��ʼ������洢
     * @param iShmKey
     * @param iSize
     */
    void initStore(size_t iStart,size_t iSize,void*shmaddr,bool isCreate)
    {
        //_shm.init(iTotalSize, iShmKey,shmaddr);
		_shmPtr = shmaddr;
        if(isCreate)
        {
            _t.create((char*)_shmPtr+iStart, iSize);
        }
        else
        {
            _t.connect((char*)_shmPtr+iStart, iSize);
        }
    }
protected:
    void * _shmPtr;
    T       _t;
};

/**
 * �ļ��洢
 */
template<typename T, typename LockPolicy>
class FileStorePolicyAuto : public LockPolicy
{
public:
    /**
     * ��ʼ���ļ�
     * @param file, �ļ�·��
     * @param iSize, �ļ���С
     */
    void initStore(const char *file, size_t iSize)
    {
        _file = file;
        _mmap.mmap(file, iSize);
        if(_mmap.iscreate())
        {
            _t.create(_mmap.getPointer(), iSize);
        }
        else
        {
            _t.connect(_mmap.getPointer(), iSize);
        }
    }

    /**
     * ��չ�ռ�, Ŀǰֻ��hashmap��Ч
     */
    int expand(size_t iSize)
    {
        TC_LockT<typename LockPolicy::Mutex> lock(LockPolicy::mutex());

        TC_Mmap m(false);
        m.mmap(_file.c_str(), iSize);

        int ret = _t.append(m.getPointer(), iSize);

        if(ret == 0)
        {
            _mmap.munmap();
            _mmap = m;
            _mmap.setOwner(true);
        }
        else
        {
            m.munmap();
        }

        return ret;
    }

protected:
    string  _file;
    TC_Mmap _mmap;
    T       _t;
};

//////////////////////////////////////////////////////////////////////
// ������: ����, �߳���, ������

/**
 * ����
 */
class EmptyLockPolicyAuto
{
public:
    typedef TC_EmptyMutex Mutex;
    Mutex &mutex()     { return _mutex; }

protected:
    Mutex _mutex;
};

/**
 * �߳�������
 */
class ThreadLockPolicyAuto
{
public:
    typedef TC_ThreadMutex Mutex;
    Mutex &mutex()     { return _mutex; }

protected:
    Mutex _mutex;
};

/**
 * ����������
 */

class SemLockPolicyAuto
{
public:
    typedef SemMutexAuto Mutex;
    void initLock(key_t iKey,unsigned short SemNum,short index)    { return _mutex.init(iKey,SemNum,index); }
    Mutex &mutex()                  { return _mutex; }

protected:
    Mutex _mutex;
};

/*
class SemLockPolicyAuto
{
public:
	typedef ProcessSem Mutex;
	//void initLock(key_t iKey,unsigned short SemNum,short index)    { return _mutex.init(iKey,SemNum,index); }
	void initLock(int pshared, unsigned int value)    { return _mutex.init(pshared,value); }

	Mutex &mutex()					{ return _mutex; }

protected:
	Mutex _mutex;
};
*/

}

#endif
