#ifndef __SEM_MUTEX_NEW_H
#define __SEM_MUTEX_NEW_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "util/tc_lock.h"
#include <semaphore.h>


namespace taf
{
/////////////////////////////////////////////////
// ˵��: �ź�������
// Author : jarodruan@tencent.com              
/////////////////////////////////////////////////
/**
* �ź������쳣��
*/
struct SemMutexAuto_Exception : public TC_Lock_Exception
{
    SemMutexAuto_Exception(const string &buffer) : TC_Lock_Exception(buffer){};
    SemMutexAuto_Exception(const string &buffer, int err) : TC_Lock_Exception(buffer, err){};
    ~SemMutexAuto_Exception() throw() {};
};

/**
* ���̼���, �ṩ����������:���������ų���
* 1 ������ͬ��key, ��ͬ���̳�ʼ��ʱ���ӵ���ͬ��sem��
* 2 ����IPC���ź���ʵ��
* 3 �ź���������SEM_UNDO����, �����̽���ʱ���Զ������ź���
*/
class SemMutexAuto
{
public:
    /**
     * ���캯��
     */
    SemMutexAuto();

    /**
    * ���캯��
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    */
    SemMutexAuto(key_t iKey,unsigned short SemNum,short index = -1);

    /**
    * ��ʼ��
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    * @return ��
     */
    void init(key_t iKey,unsigned short SemNum,short index = -1);

    /**
    * ��ȡ�����ڴ�Key
    * @return key_t ,�����ڴ�key
    */
    key_t getkey() const {return _semKey;}

    /**
    * ��ȡ�����ڴ�ID
    * @return int ,�����ڴ�Id
    */
    int getid() const   {return _semID;}

    /**
    * ��д��
    * @return int
    */
    int wlock() const;

    /**
    * ��д��
    */
    int unwlock() const;

  
    /**
    * д��
    * @return int, 0 ��ȷ
    */
    int lock() const        {return wlock();};

    /**
    * ��д��
    */
    int unlock() const      {return unwlock();};

    //�ź������
	short _index;

protected:
    /**
     * �ź���ID
     */
    int _semID;

    /**
     * �ź���key
     */
    key_t _semKey;

	  /**
     * �ź�������
     */
    int _semNum;

	
};



struct TC_ProcessSem_Exception : public TC_Lock_Exception
{
    TC_ProcessSem_Exception(const string &buffer) : TC_Lock_Exception(buffer){};
    TC_ProcessSem_Exception(const string &buffer, int err) : TC_Lock_Exception(buffer, err){};
    ~TC_ProcessSem_Exception() throw() {};
};


class ProcessSem
{
	public:
		ProcessSem() {};
		~ProcessSem();
		void init(int pshared = 1, unsigned int value = 1);
    	int lock();   
    	int unlock();    
		bool tryLock();

	private:
		sem_t _sem;
		bool _bInit;
};



}

#endif
