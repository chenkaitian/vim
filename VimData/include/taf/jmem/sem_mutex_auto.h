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
// 说明: 信号量锁类
// Author : jarodruan@tencent.com              
/////////////////////////////////////////////////
/**
* 信号量锁异常类
*/
struct SemMutexAuto_Exception : public TC_Lock_Exception
{
    SemMutexAuto_Exception(const string &buffer) : TC_Lock_Exception(buffer){};
    SemMutexAuto_Exception(const string &buffer, int err) : TC_Lock_Exception(buffer, err){};
    ~SemMutexAuto_Exception() throw() {};
};

/**
* 进程间锁, 提供两种锁机制:共享锁和排斥锁
* 1 对于相同的key, 不同进程初始化时连接到相同的sem上
* 2 采用IPC的信号量实现
* 3 信号量采用了SEM_UNDO参数, 当进程结束时会自动调整信号量
*/
class SemMutexAuto
{
public:
    /**
     * 构造函数
     */
    SemMutexAuto();

    /**
    * 构造函数
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    */
    SemMutexAuto(key_t iKey,unsigned short SemNum,short index = -1);

    /**
    * 初始化
    * @param iKey, key
    * @throws TC_SemMutex_Exception
    * @return 无
     */
    void init(key_t iKey,unsigned short SemNum,short index = -1);

    /**
    * 获取共享内存Key
    * @return key_t ,共享内存key
    */
    key_t getkey() const {return _semKey;}

    /**
    * 获取共享内存ID
    * @return int ,共享内存Id
    */
    int getid() const   {return _semID;}

    /**
    * 加写锁
    * @return int
    */
    int wlock() const;

    /**
    * 解写锁
    */
    int unwlock() const;

  
    /**
    * 写锁
    * @return int, 0 正确
    */
    int lock() const        {return wlock();};

    /**
    * 解写锁
    */
    int unlock() const      {return unwlock();};

    //信号量编号
	short _index;

protected:
    /**
     * 信号量ID
     */
    int _semID;

    /**
     * 信号量key
     */
    key_t _semKey;

	  /**
     * 信号量个数
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
