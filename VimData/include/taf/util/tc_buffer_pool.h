#ifndef __TC_BUFFERPOOL_H
#define __TC_BUFFERPOOL_H

#include <list>
#include <vector>
#include <string>


namespace taf
{

/////////////////////////////////////////////////
/** 
 *@file   tc_buffer_pool.h
 *@brief  缓冲池封装类. 
 * 
 *@author okochayang@tencent.com
 */
/////////////////////////////////////////////////


struct TC_Slice
{
    explicit TC_Slice(void* d = NULL , size_t ds = 0, size_t l = 0);
    void* data;
    size_t dataLen;
    size_t len;

};

/** 
 *@brief  缓冲池封装类. 
 */
class TC_BufferPool
{
public:
   /**
	* @brief 构造函数. 
	* @param minBlock 该缓冲池所负责分配的最小内存块大小
	* @param maxBlock 该缓冲池所负责分配的最大内存块大小
    */
    TC_BufferPool(size_t minBlock, size_t maxBlock);

   /**
    * @brief 析构函数. 
    */
    ~TC_BufferPool();

   /**
    * @brief 内存分配
	* @param size 请求分配的内存块大小
	* @return 分配的内存地址及大小
    */
    TC_Slice Allocate(size_t size);

   /**
    * @brief 内存释放
	* @param s 请求释放的内存块大小
    */
    void Deallocate(TC_Slice s);

   /**
    * @brief 设置pool中内存占用上限
	* @param bytes 上限
    */
    void SetMaxBytes(size_t bytes);

   /**
    * @brief 获取pool中内存占用上限
	* reutrn 上限
    */
    size_t GetMaxBytes() const;

   /**
    * @brief 调试打印
	* reutrn 打印的字符串
    */
    std::string DebugPrint() const;

private:
    typedef std::list<void* > BufferList;

   /**
    * @brief 从指定的buffer链表获取内存
	* @param size 请求分配的内存块大小
	* @param blist 请求的buffer链表
	* @return 分配的内存地址及大小
    */
    TC_Slice _Allocate(size_t size, BufferList& blist);

   /**
    * @brief 根据指定的分配尺寸，寻找合适的buffer链表
	* @param s 请求分配的内存块大小
	* @return buffer链表
    */
    BufferList& _GetBufferList(size_t s);

   /**
    * @brief 根据指定的分配尺寸，寻找合适的buffer链表
	* @param s 请求分配的内存块大小
	* @return buffer链表
    */
    const BufferList& _GetBufferList(size_t s) const;

    /**
     * buffer链表池
     */
    std::vector<BufferList> buffers_;

    /**
     * buffer链表池负责分配的最小内存块
     */
    const size_t minBlock_;

    /**
     * buffer链表池负责分配的最大内存块
     */
    const size_t maxBlock_;

    /**
     * buffer链表池占用的内存上限
     */
    size_t maxBytes_;

    /**
     * buffer链表池目前占用的内存
     */
    size_t totalBytes_;
};

} //end namespace taf

#endif

