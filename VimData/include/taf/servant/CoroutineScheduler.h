#ifndef _COROUTINES_SCHEDULLER_H_
#define _COROUTINES_SCHEDULLER_H_

#include <cstddef>
#include <list>
#include <set>
#include <deque>
#include <map>
#include "util/tc_fcontext.h"
#include "util/tc_thread_queue.h"
#include "util/tc_monitor.h"
#include "util/tc_callback.h"
#include "util/tc_bind.h"
#include "util/tc_thread.h"

using namespace std;

namespace taf
{
class ServantHandle;

/////////////////////////////////////////////
//协程使用的栈内容信息
struct stack_context
{
    std::size_t	size;
    void*		sp;

    stack_context()
	: size(0)
	, sp(0)
    {}
};

/////////////////////////////////////////////
//协程使用的栈内存分配器
class standard_stack_allocator
{
public:
    static bool is_stack_unbound();

    static std::size_t default_stacksize();

    static std::size_t minimum_stacksize();

    static std::size_t maximum_stacksize();

    int allocate( stack_context &, std::size_t);

    void deallocate( stack_context &);
};

/////////////////////////////////////////////
//协程的状态信息
enum CORO_STATUS
{
	CORO_FREE		= 0,
	CORO_ACTIVE		= 1,
	CORO_AVAIL		= 2,
	CORO_INACTIVE	= 3,
	CORO_TIMEOUT	= 4
};

/////////////////////////////////////////////
//协程内部使用的函数
typedef void( * Func)(void *);

struct CoroutineFunc
{
	Func	coroFunc;
	void*	args;
};

class CoroutineScheduler;

///////////////////////////////////////////
////协程信息类
class CoroutineInfo
{
public:
	static inline void CoroutineHeadInit(CoroutineInfo *coro)
	{
		coro->_next = coro;
		coro->_prev = coro;
	}

	static inline bool CoroutineHeadEmpty(CoroutineInfo *coro_head)
	{
		return coro_head->_next == coro_head;
	}

	static inline void __CoroutineAdd(CoroutineInfo *coro, CoroutineInfo *prev, CoroutineInfo *next)
	{
		next->_prev = coro;
		coro->_next = next;
		coro->_prev = prev;
		prev->_next = coro;
	}

	static inline void CoroutineAdd(CoroutineInfo *new_coro, CoroutineInfo *coro_head)
	{
		__CoroutineAdd(new_coro, coro_head, coro_head->_next);
	}

	static inline void CoroutineAddTail(CoroutineInfo *new_coro, CoroutineInfo *coro_head)
	{
		__CoroutineAdd(new_coro, coro_head->_prev, coro_head);
	}

	static inline void __CoroutineDel(CoroutineInfo * prev, CoroutineInfo * next)
	{
		next->_prev = prev;
		prev->_next = next;
	}

	static inline void CoroutineDel(CoroutineInfo *coro)
	{
		__CoroutineDel(coro->_prev, coro->_next);
		coro->_next = NULL;
		coro->_prev = NULL;
	}

	static inline void CoroutineMove(CoroutineInfo *coro, CoroutineInfo *coro_head)
	{
		CoroutineDel(coro);
		CoroutineAdd(coro, coro_head);
	}

	static inline void CoroutineMoveTail(CoroutineInfo *coro, CoroutineInfo *coro_head)
	{
		CoroutineDel(coro);
		CoroutineAddTail(coro, coro_head);
	}

protected:

	//协程的入口函数
	static void corotineEntry(intptr_t q);

	//在协程里执行实际逻辑的入口函数
	static void corotineProc(void * args);

public:
	//构造函数
	CoroutineInfo();

	//构造函数
	CoroutineInfo(CoroutineScheduler* scheduler);

	//构造函数
	CoroutineInfo(CoroutineScheduler* scheduler, uint32_t iUid, stack_context stack_ctx);

	//析构函数
	~CoroutineInfo();

	//注册协程实际的处理函数
	void registerFunc(const taf::TC_Callback<void ()> &callback);

	//设置协程的内存空间
	void setStackContext(stack_context stack_ctx);

	//获取协程的内存空间
	inline stack_context& getStackContext() { return _stack_ctx; }

	//获取协程所处的调度器
	inline CoroutineScheduler* getScheduler() { return _scheduler; }

	//获取协程的标志
	inline uint32_t getUid() { return _uid; }

	//设置协程的标志
	inline void setUid(uint32_t iUid) { _uid = iUid; }

	//获取协程的状态
	inline CORO_STATUS getStatus() { return _eStatus; }

	//设置协程的状态
	inline void setStatus(CORO_STATUS status) { _eStatus = status; }

	//获取协程所处的上下文
	inline fcontext_t* getCtx() { return (!_bMain ? _ctx_to : &_ctx_from); }

public:
	CoroutineInfo*				_prev;
	CoroutineInfo*				_next;

private:
	//是否是主协程
	bool						_bMain;

	//协程所属的调度器
	CoroutineScheduler*			_scheduler;

	//协程的标识
	uint32_t					_uid;

	//协程的状态
	CORO_STATUS					_eStatus;

	//协程的内存空间
	stack_context				_stack_ctx;

	//创建协程后，协程所在的上下文
	fcontext_t*					_ctx_to;

	//创建协程前的上下文
    fcontext_t					_ctx_from;

	//协程初始化函数入口函数
	CoroutineFunc				_init_func;

	//协程具体执行函数
	taf::TC_Callback<void ()>	_callback;
};

///////////////////////////////////////////
//协程调度类
class CoroutineScheduler
{	
public:

	//构造函数
	CoroutineScheduler();

	//析构函数
	~CoroutineScheduler();

	//初始化协程池的大小、以及协程的堆栈大小
	void init(uint32_t iPoolSize, size_t iStackSize);

	//创建协程
	uint32_t createCoroutine(const taf::TC_Callback<void ()> &callback);

	//在用户自己起的线程中使用协程时，用到的协程调度
	void run();

	//taf框架业务线程使用协程时，用的协程调度
	void taf_run();

	//当前协程放弃继续执行
	void yield(bool bFlag = true);

	//当前协程休眠iSleepTime时间(单位:毫秒)，然后会被唤醒继续执行
	void sleep(int iSleepTime);

	//放入需要唤醒的协程
	void put(uint32_t iCoroId);

	//协程切换
	void switchCoro(CoroutineInfo *from, CoroutineInfo *to);

	//停止
	void terminate();

	//资源销毁
	void destroy();

	//获取最大的协程数目
	inline uint32_t getPoolSize() { return _iPoolSize; }

	//获取当前已经创建的协程数目
	inline uint32_t getCurrentSize() { return _iCurrentSize; }

	//获取请求响应回来的协程数目
	inline size_t getResponseCoroSize() { return _activeCoroQueue.size(); }

	//获取框架业务线程的Handle
	inline ServantHandle* getHandle() { return _handle; }

	//设置框架业务线程的Handle
	inline void setHandle(ServantHandle* handle) { _handle = handle; }

	//获取理论上空闲的协程数目
	inline uint32_t getFreeSize() { return _iPoolSize - _iUsedSize; }

	//减少正在使用的协程数目
	inline void decUsedSize() { --_iUsedSize; }

	//增加正在使用的协程数目
	inline void incUsedSize() { ++_iUsedSize; }

	//调度器中的主协程
	inline CoroutineInfo& getMainCoroutine() { return _mainCoro; }

	//当前协程的标识Id
	inline uint32_t getCoroutineId() { return _currentCoro->getUid(); }

	friend class CoroutineInfo;

private:
	//产生协程id
	uint32_t generateId();

	//增加协程池的大小
	int	increaseCoroPoolSize();

	//唤醒需要运行的协程
	void wakeup();

	//唤醒自己放弃运行的协程
	void wakeupbyself();

	//自己放弃运行时，用到
	void putbyself(uint32_t iCoroId);

	//唤醒休眠的协程
	void wakeupbytimeout();

	//放到active的协程链表中
	void moveToActive(CoroutineInfo *coro, bool bFlag = false);

	//放到avail的协程链表中
	void moveToAvail(CoroutineInfo *coro);

	//放到inactive的协程链表中
	void moveToInactive(CoroutineInfo *coro);

	//放到超时等待的协程链表中
	void moveToTimeout(CoroutineInfo *coro);

	//放到空闲的协程链表中
	void moveToFreeList(CoroutineInfo *coro);

private:
	bool				_bTerminal;
	uint32_t			_iPoolSize;
	size_t				_iStackSize;
	uint32_t			_iCurrentSize;
	uint32_t			_iUsedSize;
	uint32_t			_iUniqId;

	ServantHandle		*_handle;

	uint32_t			_iCurrentCoroUid;
	CoroutineInfo			_mainCoro;
	CoroutineInfo*			_currentCoro;

	CoroutineInfo**			_all_coro;

	CoroutineInfo			_active;
	CoroutineInfo			_avail;
	CoroutineInfo			_inactive;
	CoroutineInfo			_timeout;
	CoroutineInfo			_free;

	standard_stack_allocator alloc;

	TC_ThreadQueue<uint32_t, deque<uint32_t> >	_activeCoroQueue;

	TC_ThreadLock		_monitor;

	list<uint32_t>		_needActiveCoroId;

	multimap<int64_t, uint32_t> _mTimeoutCoroId;
};

/////////////////////////////////////////////////////////////////////
/**
 * @brief  协程异常类
 */
struct CoroutineException : public TC_Exception
{
    CoroutineException(const string &buffer) : TC_Exception(buffer){};
    CoroutineException(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~CoroutineException() throw() {};
};

//对线程进行包装的协程类，主要用于在自己起的线程中使用协程,业务可以继承这个类
class Coroutine : public TC_Thread
{
public:
	/**
     * 构造函数
     */
	Coroutine();

	/**
     * 析构函数
     */
	virtual ~Coroutine();

	/**
     * 初始化
	 * @iNum, 表示要运行多个协程，即会有这个类多少个coroRun运行
	 * @iTotalNum，表示这个线程最多包含的协程个数
	 * @iStackSize，协程的栈大小
     */
	void setCoroInfo(uint32_t iNum, uint32_t iMaxNum, size_t iStackSize);

	/**
	 * 创建协程，在已经创建的协程中使用
	 * 返回值为协程的id，大于0，表示成功，，小于等于0，表示失败
	 */
	uint32_t createCoroutine(const taf::TC_Callback<void ()> &coroFunc);

	/**
	 * 当前协程自己放弃执行,会自动被调度器唤醒
	 * 在已经创建的协程中使用
	 */
	void yield();

	/**
	 * 当前协程休眠iSleepTime时间(单位:毫秒)，时间到了，会自动被调度器唤醒
	 * 在已经创建的协程中使用
	 */
	void Sleep(int iSleepTime);

	/**
     * 返回协程调度类的指针
     */
	CoroutineScheduler * getCoroSched() { return _coroSched; }

	/**
     * 获取设置的最大协程的数目
     */
	uint32_t getMaxCoroNum() { return _iMaxNum; }

	/**
     * 获取启动时，设置的协程的数目
     */
	uint32_t getCoroNum() { return _iNum; }

	/**
     * 设置协程的栈大小
     */
	size_t getCoroStackSize() { return _iStackSize; }

	/**
	 * 停止
	 */
	void terminate();

protected:

	/**
	 * 线程处理方法
	 */
	virtual void run();

	/**
	 *  静态函数, 协程入口. 
	 */
	static void coroEntry(Coroutine *pCoro);

	/**
     * 协程运行的函数，根据_iNum的数目，会启动_iNum个这个函数
	 */
    virtual void handle() = 0;

protected:
	/**
	 * 线程已经启动, 进入协程处理前调用
	 */
	virtual void initialize() {}

	/**
	 * 所有协程停止运行之后，线程退出之前时调用
	 */
	virtual void destroy() {}

	/**
	 * 具体的处理逻辑
	 */
	virtual void handleCoro();

private:
	CoroutineScheduler *_coroSched;
	uint32_t			_iNum;
	uint32_t			_iMaxNum;
	size_t				_iStackSize;
};

}

#endif
