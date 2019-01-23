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
//Э��ʹ�õ�ջ������Ϣ
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
//Э��ʹ�õ�ջ�ڴ������
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
//Э�̵�״̬��Ϣ
enum CORO_STATUS
{
	CORO_FREE		= 0,
	CORO_ACTIVE		= 1,
	CORO_AVAIL		= 2,
	CORO_INACTIVE	= 3,
	CORO_TIMEOUT	= 4
};

/////////////////////////////////////////////
//Э���ڲ�ʹ�õĺ���
typedef void( * Func)(void *);

struct CoroutineFunc
{
	Func	coroFunc;
	void*	args;
};

class CoroutineScheduler;

///////////////////////////////////////////
////Э����Ϣ��
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

	//Э�̵���ں���
	static void corotineEntry(intptr_t q);

	//��Э����ִ��ʵ���߼�����ں���
	static void corotineProc(void * args);

public:
	//���캯��
	CoroutineInfo();

	//���캯��
	CoroutineInfo(CoroutineScheduler* scheduler);

	//���캯��
	CoroutineInfo(CoroutineScheduler* scheduler, uint32_t iUid, stack_context stack_ctx);

	//��������
	~CoroutineInfo();

	//ע��Э��ʵ�ʵĴ�����
	void registerFunc(const taf::TC_Callback<void ()> &callback);

	//����Э�̵��ڴ�ռ�
	void setStackContext(stack_context stack_ctx);

	//��ȡЭ�̵��ڴ�ռ�
	inline stack_context& getStackContext() { return _stack_ctx; }

	//��ȡЭ�������ĵ�����
	inline CoroutineScheduler* getScheduler() { return _scheduler; }

	//��ȡЭ�̵ı�־
	inline uint32_t getUid() { return _uid; }

	//����Э�̵ı�־
	inline void setUid(uint32_t iUid) { _uid = iUid; }

	//��ȡЭ�̵�״̬
	inline CORO_STATUS getStatus() { return _eStatus; }

	//����Э�̵�״̬
	inline void setStatus(CORO_STATUS status) { _eStatus = status; }

	//��ȡЭ��������������
	inline fcontext_t* getCtx() { return (!_bMain ? _ctx_to : &_ctx_from); }

public:
	CoroutineInfo*				_prev;
	CoroutineInfo*				_next;

private:
	//�Ƿ�����Э��
	bool						_bMain;

	//Э�������ĵ�����
	CoroutineScheduler*			_scheduler;

	//Э�̵ı�ʶ
	uint32_t					_uid;

	//Э�̵�״̬
	CORO_STATUS					_eStatus;

	//Э�̵��ڴ�ռ�
	stack_context				_stack_ctx;

	//����Э�̺�Э�����ڵ�������
	fcontext_t*					_ctx_to;

	//����Э��ǰ��������
    fcontext_t					_ctx_from;

	//Э�̳�ʼ��������ں���
	CoroutineFunc				_init_func;

	//Э�̾���ִ�к���
	taf::TC_Callback<void ()>	_callback;
};

///////////////////////////////////////////
//Э�̵�����
class CoroutineScheduler
{	
public:

	//���캯��
	CoroutineScheduler();

	//��������
	~CoroutineScheduler();

	//��ʼ��Э�̳صĴ�С���Լ�Э�̵Ķ�ջ��С
	void init(uint32_t iPoolSize, size_t iStackSize);

	//����Э��
	uint32_t createCoroutine(const taf::TC_Callback<void ()> &callback);

	//���û��Լ�����߳���ʹ��Э��ʱ���õ���Э�̵���
	void run();

	//taf���ҵ���߳�ʹ��Э��ʱ���õ�Э�̵���
	void taf_run();

	//��ǰЭ�̷�������ִ��
	void yield(bool bFlag = true);

	//��ǰЭ������iSleepTimeʱ��(��λ:����)��Ȼ��ᱻ���Ѽ���ִ��
	void sleep(int iSleepTime);

	//������Ҫ���ѵ�Э��
	void put(uint32_t iCoroId);

	//Э���л�
	void switchCoro(CoroutineInfo *from, CoroutineInfo *to);

	//ֹͣ
	void terminate();

	//��Դ����
	void destroy();

	//��ȡ����Э����Ŀ
	inline uint32_t getPoolSize() { return _iPoolSize; }

	//��ȡ��ǰ�Ѿ�������Э����Ŀ
	inline uint32_t getCurrentSize() { return _iCurrentSize; }

	//��ȡ������Ӧ������Э����Ŀ
	inline size_t getResponseCoroSize() { return _activeCoroQueue.size(); }

	//��ȡ���ҵ���̵߳�Handle
	inline ServantHandle* getHandle() { return _handle; }

	//���ÿ��ҵ���̵߳�Handle
	inline void setHandle(ServantHandle* handle) { _handle = handle; }

	//��ȡ�����Ͽ��е�Э����Ŀ
	inline uint32_t getFreeSize() { return _iPoolSize - _iUsedSize; }

	//��������ʹ�õ�Э����Ŀ
	inline void decUsedSize() { --_iUsedSize; }

	//��������ʹ�õ�Э����Ŀ
	inline void incUsedSize() { ++_iUsedSize; }

	//�������е���Э��
	inline CoroutineInfo& getMainCoroutine() { return _mainCoro; }

	//��ǰЭ�̵ı�ʶId
	inline uint32_t getCoroutineId() { return _currentCoro->getUid(); }

	friend class CoroutineInfo;

private:
	//����Э��id
	uint32_t generateId();

	//����Э�̳صĴ�С
	int	increaseCoroPoolSize();

	//������Ҫ���е�Э��
	void wakeup();

	//�����Լ��������е�Э��
	void wakeupbyself();

	//�Լ���������ʱ���õ�
	void putbyself(uint32_t iCoroId);

	//�������ߵ�Э��
	void wakeupbytimeout();

	//�ŵ�active��Э��������
	void moveToActive(CoroutineInfo *coro, bool bFlag = false);

	//�ŵ�avail��Э��������
	void moveToAvail(CoroutineInfo *coro);

	//�ŵ�inactive��Э��������
	void moveToInactive(CoroutineInfo *coro);

	//�ŵ���ʱ�ȴ���Э��������
	void moveToTimeout(CoroutineInfo *coro);

	//�ŵ����е�Э��������
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
 * @brief  Э���쳣��
 */
struct CoroutineException : public TC_Exception
{
    CoroutineException(const string &buffer) : TC_Exception(buffer){};
    CoroutineException(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~CoroutineException() throw() {};
};

//���߳̽��а�װ��Э���࣬��Ҫ�������Լ�����߳���ʹ��Э��,ҵ����Լ̳������
class Coroutine : public TC_Thread
{
public:
	/**
     * ���캯��
     */
	Coroutine();

	/**
     * ��������
     */
	virtual ~Coroutine();

	/**
     * ��ʼ��
	 * @iNum, ��ʾҪ���ж��Э�̣��������������ٸ�coroRun����
	 * @iTotalNum����ʾ����߳���������Э�̸���
	 * @iStackSize��Э�̵�ջ��С
     */
	void setCoroInfo(uint32_t iNum, uint32_t iMaxNum, size_t iStackSize);

	/**
	 * ����Э�̣����Ѿ�������Э����ʹ��
	 * ����ֵΪЭ�̵�id������0����ʾ�ɹ�����С�ڵ���0����ʾʧ��
	 */
	uint32_t createCoroutine(const taf::TC_Callback<void ()> &coroFunc);

	/**
	 * ��ǰЭ���Լ�����ִ��,���Զ�������������
	 * ���Ѿ�������Э����ʹ��
	 */
	void yield();

	/**
	 * ��ǰЭ������iSleepTimeʱ��(��λ:����)��ʱ�䵽�ˣ����Զ�������������
	 * ���Ѿ�������Э����ʹ��
	 */
	void Sleep(int iSleepTime);

	/**
     * ����Э�̵������ָ��
     */
	CoroutineScheduler * getCoroSched() { return _coroSched; }

	/**
     * ��ȡ���õ����Э�̵���Ŀ
     */
	uint32_t getMaxCoroNum() { return _iMaxNum; }

	/**
     * ��ȡ����ʱ�����õ�Э�̵���Ŀ
     */
	uint32_t getCoroNum() { return _iNum; }

	/**
     * ����Э�̵�ջ��С
     */
	size_t getCoroStackSize() { return _iStackSize; }

	/**
	 * ֹͣ
	 */
	void terminate();

protected:

	/**
	 * �̴߳�����
	 */
	virtual void run();

	/**
	 *  ��̬����, Э�����. 
	 */
	static void coroEntry(Coroutine *pCoro);

	/**
     * Э�����еĺ���������_iNum����Ŀ��������_iNum���������
	 */
    virtual void handle() = 0;

protected:
	/**
	 * �߳��Ѿ�����, ����Э�̴���ǰ����
	 */
	virtual void initialize() {}

	/**
	 * ����Э��ֹͣ����֮���߳��˳�֮ǰʱ����
	 */
	virtual void destroy() {}

	/**
	 * ����Ĵ����߼�
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
