#ifndef AUTO_JMEM_HASHMAP_MALLOC_H
#define AUTO_JMEM_HASHMAP_MALLOC_H

#include "jmem_hashmap_malloc.h"
#include "tc_hashmap_malloc.h"
#include "util/tc_shm.h"
#include "NormalHash.h"
#include <math.h>
namespace taf
{
	template<typename LockPolicy,
         template<class, class> class StorePolicy>
	class JceHashMapMallocAuto
	{
	    public:
			typedef JceHashMapMalloc<LockPolicy, StorePolicy> JmemHashMap;
			typedef typename JmemHashMap::ToDoFunctor CacheToDoFunctor;
			typedef typename JmemHashMap::ToDoFunctor::DataRecord CacheDataRecord;
			typedef typename JmemHashMap::hash_iterator Jmem_hash_iterator;
			typedef typename JmemHashMap::JhmItem  Jmem_hash_Item;
		///////////////////////////////////////////////////////////////
		/**
		 * 迭代器
		 */


		struct JmemHashAutoIterator
		{
		public:

			/**
			 * 构造
			 * @param it
			 * @param lock
			 */
			JmemHashAutoIterator(JceHashMapMallocAuto<LockPolicy, StorePolicy> * pMap,const Jmem_hash_iterator & itr,unsigned short jmemIndex,unsigned short jmemNum)
			: _pMap(pMap), _itr(itr), _jmemIndex(jmemIndex),_jmemNum(jmemNum)
			{
			}

			/**
			 * 拷贝构造
			 * @param it
			 */
			JmemHashAutoIterator(const JmemHashAutoIterator &it)
			: _pMap(it._pMap), _itr(it._itr), _jmemIndex(it._jmemIndex),_jmemNum(it._jmemNum)
			{
			}

			/**
			 * 复制
			 * @param it
			 *
			 * @return JhmIterator&
			 */
			JmemHashAutoIterator& operator=(const JmemHashAutoIterator &it)
			{
				if(this != &it)
				{
					_pMap     = it._pMap;
					_itr   = it._itr;
					_jmemIndex = it._jmemIndex;
					_jmemNum = it._jmemNum;
				}

				return (*this);
			}

			/**
			 *
			 * @param it
			 *
			 * @return bool
			 */
			bool operator==(const JmemHashAutoIterator& it)
			{
				return (_pMap == it._pMap && _itr == it._itr && _jmemIndex == it._jmemIndex && _jmemNum == it._jmemNum);
			}

			/**
			 *
			 * @param mv
			 *
			 * @return bool
			 */
			bool operator!=(const JmemHashAutoIterator& it)
			{
				return !((*this) == it);
			}

    		/**
    		 * 前置++
    		 *
    		 * @return JhmIterator&
    		 */
    		JmemHashAutoIterator& operator++()
			{
				++_itr;
				if(_itr == _pMap->_jceHashMapVec[_jmemIndex]->hashEnd())
				{
					if(_jmemIndex != (_jmemNum-1))
					{
						_jmemIndex++;
						_itr = _pMap->_jceHashMapVec[_jmemIndex]->hashBegin();
						//LOG->debug() <<"JmemHashAutoIterator ++ Index"<<_jmemIndex<<endl;
					}
					/*
					else
					{
					    LOG->debug() <<"JmemHashAutoIterator ++ to end"<<_jmemIndex<<endl;
					}
					*/
				}
				
				return (*this);
			}

    		/**
			 * 后置++
			 *
			 * @return JhmIterator&
    		 */
    		JmemHashAutoIterator operator++(int)
			{
				JmemHashAutoIterator itTmp(*this);
				++_itr;
				if(_itr == _pMap->_jceHashMapVec[_jmemIndex]->hashEnd())
				{
					if(_jmemIndex != (_jmemNum-1))
					{
						_jmemIndex++;
						_itr = _pMap->_jceHashMapVec[_jmemIndex]->hashBegin();
					}
				}
				return itTmp;
			}

            //设置索引值
            bool setIndex(uint32_t index)
			{
                return _itr.setIndex(index);
			}

    		/**
			 * 获取数据项
    		 *
    		 * @return JhmItem&
    		 */
    		 Jmem_hash_Item& operator*()     { return *_itr; }

    		/**
			 * 获取数据项
    		 *
    		 * @return JhmItem*
    		 */
    		 Jmem_hash_Item* operator->()   { return &(*_itr); }

		protected:
			JceHashMapMallocAuto<LockPolicy, StorePolicy> * _pMap;

			Jmem_hash_iterator  _itr;

			unsigned short _jmemIndex;

			unsigned short _jmemNum;
		};

		//typedef JhmIterator hash_iterator;
		typedef JmemHashAutoIterator auto_hash_iterator;
	public:
		JceHashMapMallocAuto()
		{
			_pHash = new NormalHash();
		}
		~JceHashMapMallocAuto()
		{
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				 if(_jceHashMapVec[i]!=NULL)
				 {
				     delete _jceHashMapVec[i];
				 }
			 }
			 if(_pHash!=NULL)
			 {
			    delete _pHash;
			 }
		}
		void init(unsigned int jmemNum)
		{
			_jceHashMapVec.clear();
			_jmemNum = jmemNum;
			for(unsigned int i = 0;i<jmemNum;i++)
			{
			   JmemHashMap * jceHashMapPtr = new JmemHashMap();
			   _jceHashMapVec.push_back(jceHashMapPtr);
			}
		}
		void initHashRadio(float fRadio)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->initHashRadio(fRadio);

			}
		}
		void initAvgDataSize(size_t iAvgDataSize)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->initAvgDataSize(iAvgDataSize);
			}
		}

		void initStore(key_t keyShm,size_t length)
		{
			LOG->debug() <<"initStore start"<<endl;
			_shm.init(length, keyShm);
			size_t oneJmemLength = length/_jmemNum;
			LOG->debug() <<"_jmemNum=" << _jmemNum << "|oneJmemLength="<<oneJmemLength<<endl;

			 if(_shm.iscreate())
            {
				for(size_t i = 0;i<_jmemNum;i++)
				{
					_jceHashMapVec[i]->initStore(i*oneJmemLength,oneJmemLength,_shm.getPointer(),true);
				}
			 }
			 else
			 {
				 for(size_t i = 0;i<_jmemNum;i++)
				{
					_jceHashMapVec[i]->initStore(i*oneJmemLength,oneJmemLength,_shm.getPointer(),false);
				}
			 }
			 LOG->debug() <<"initStore finish"<<endl;
		}

		void initLock(key_t iKey,unsigned short SemNum,short index)
		{
			_Mutex.init(iKey,SemNum,index);
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->initLock(iKey,SemNum,i);
				//LOG->debug() <<short(i)<<"SemKeyNumStore" <<iKey <<"SemNum"<<SemNum<<endl;
			}
			LOG->debug() <<"initLock finish"<<endl;
		}
		void setSyncTime(uint32_t iSyncTime)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->setSyncTime(iSyncTime);
			}
			LOG->debug() <<"setSyncTime finish"<<endl;
		}

	    void setToDoFunctor(CacheToDoFunctor *todo_of)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->setToDoFunctor(todo_of);
			}
			LOG->debug() <<"setToDoFunctor finish"<<endl;
		}

		void setHashFunctor(TC_HashMapMalloc::hash_functor hashf)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->setHashFunctor(hashf);
			}
			LOG->debug() <<"setHashFunctor finish"<<endl;
		}
 		void setAutoErase(bool bAutoErase)
		{
			for(size_t i = 0;i<_jmemNum;i++)
			{
				_jceHashMapVec[i]->setAutoErase(bAutoErase);
			}
			LOG->debug() <<"setAutoErase finish"<<endl;
		}

		int eraseByForce(const string& k)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->eraseByForce(k);
		}

		int set(const string& k, const string& v, bool bDirty = true, uint32_t iExpireTime = 0, uint8_t iVersion = 0)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->set(k,v,bDirty,iExpireTime,iVersion);
		}

		int set(const string& k, uint8_t iVersion = 0)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->set(k,iVersion);
		}

        int update(const string& k, const string& v, TafOp option, bool bDirty = true, uint32_t iExpireTime = 0,bool bCheckExpire = false, uint32_t iNowTime = -1, string &retValue = "")
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->update(k,v,option,bDirty,iExpireTime,bCheckExpire,iNowTime, retValue);
		}

		int del(const string& k)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->del(k);
		}

        int del(const string& k, const uint8_t iVersion)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->del(k, iVersion);
		}

		int delExpire(const string& k)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->delExpire(k);
		}

		int get(const string& k, string &v, uint32_t &iSyncTime, uint32_t& iExpireTime, uint8_t& iVersion, bool bCheckExpire = false, uint32_t iNowTime = -1)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->get(k, v, iSyncTime, iExpireTime, iVersion, bCheckExpire, iNowTime);
		}

		int get(const string& k, string &v, uint32_t &iSyncTime, uint32_t& iExpireTime, uint8_t& iVersion, bool& bDirty,bool bCheckExpire = false, uint32_t iNowTime = -1)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->get(k, v, iSyncTime, iExpireTime, iVersion, bDirty, bCheckExpire, iNowTime);
		}

		int get(const string& k, string &v, bool bCheckExpire = false, uint32_t iNowTime = -1)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->get(k,v,bCheckExpire,iNowTime);
		}

		int checkDirty(const string &k, bool bCheckExpire = false, uint32_t iNowTime = -1)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->checkDirty(k,bCheckExpire,iNowTime);
		}

		int erase(const string& k)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->erase(k);
		}

        int erase(const string& k, const uint8_t iVersion)
		{
			return _jceHashMapVec[_pHash->HashRawString(k)%_jmemNum]->erase(k, iVersion);
		}

		template<typename C>
	    int eraseHashByForce(size_t h, C c)
		{
			return _jceHashMapVec[h%_jmemNum]->eraseHashByForce<C>(h,c);
		}

		template<typename C>
	    int eraseHashByForce(size_t h, C c, vector<string>& vDelK)
		{
			return _jceHashMapVec[h%_jmemNum]->eraseHashByForce<C>(h,c,vDelK);
		}

		template<typename C>
        int getHash(size_t h, vector<pair<string, string> > &vv, C c)
		{
			return _jceHashMapVec[h%_jmemNum]->getHash<C>(h,vv,c);
		}

		template<typename C>
        int getHash(size_t h, vector<CacheDataRecord> &vv, C c)
		{
			return _jceHashMapVec[h%_jmemNum]->getHash<C>(h,vv,c);
		}

		template<typename C>
        int getHashWithOnlyKey(size_t h, vector<CacheDataRecord> &vv, C c)
		{
			return _jceHashMapVec[h%_jmemNum]->getHashWithOnlyKey<C>(h,vv,c);
		}

		void getMapHead(vector<TC_HashMapMalloc::tagMapHead> & headVtr)                    
		{ 
			headVtr.clear();
			for(size_t i = 0;i<_jmemNum;i++)
			{
				headVtr.push_back(_jceHashMapVec[i]->getMapHead());
			}
		}

		 size_t getHashCount()
		 {
			 size_t hashCount = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				hashCount += _jceHashMapVec[i]->getHashCount();
			 }
			 return hashCount;
		 }

		 void clear()
		 {
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				_jceHashMapVec[i]->clear();
			 }
		 }

		 int erase(int radio, bool bCheckDirty = false)
		 {
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				_jceHashMapVec[i]->erase(radio,bCheckDirty);
			 }
			 return 0;
		 }

		 int getUseRatio(unsigned int uJmemIndex,int& iRatio)
		 {
		 	if (uJmemIndex >= _jmemNum)
		 	{
		 		//LOG->error() << "eraseByID getUseRatio fail.uJmemIndex=" << uJmemIndex << " larger than jMemCount=" << _jceHashMapVec.size() << endl;
		 		return -1;
		 	}	

			iRatio = _jceHashMapVec[uJmemIndex]->getMapHead()._iUsedDataMem * 100/ _jceHashMapVec[uJmemIndex]->getDataMemSize();
		 	//LOG->debug() << "eraseByID getUseRatio.radio=" << fRatio << "|uJmemIndex=" << uJmemIndex << endl;
			return 0;
		 }

		 int eraseByID(int radio, unsigned int uJmemIndex,unsigned int uMaxEraseOneTime,bool bCheckDirty = false)
		 {
		 	//LOG->debug() << "eraseByID enter.radio=" << radio << "|uJmemIndex=" << uJmemIndex << endl;
		 	if (uJmemIndex >= _jmemNum)
		 	{
		 		//LOG->error() << "eraseByID fail.uJmemIndex=" << uJmemIndex << " larger than jMemCount=" << _jceHashMapVec.size() << endl;
		 		return -1;
		 	}
			return _jceHashMapVec[uJmemIndex]->erase(radio,uMaxEraseOneTime,bCheckDirty);
		 }		 

		 size_t dirtyCount()
		 {
			 size_t dirtyCount = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				dirtyCount += _jceHashMapVec[i]->dirtyCount();
			 }
			 return dirtyCount;
		 }

		 size_t size()
		 {
			 size_t eleCount = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				eleCount += _jceHashMapVec[i]->size();
			 }
			 return eleCount;
		 }

		 size_t onlyKeyCount()
		 {
			 size_t onlyKeyCount = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				onlyKeyCount += _jceHashMapVec[i]->onlyKeyCount();
			 }
			 return onlyKeyCount;
		 }

		 size_t usedChunkCount()
		 {
             size_t usedChunkCount = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				usedChunkCount += _jceHashMapVec[i]->usedChunkCount();
			 }
			 return usedChunkCount;
		 }

		 void sync()
		 {
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				_jceHashMapVec[i]->sync();
			 }
		 }

		 template<typename C>
         int syncOnce(uint32_t iNowTime, C c)
		 {
			 vector<int> iRetVec(_jmemNum,0);
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				iRetVec[i] = _jceHashMapVec[i]->syncOnce<C>(iNowTime,c);
			 }
			 bool bOK = true;
			 bool bError = false;
			 for(size_t i = 0;i<iRetVec.size();i++)
			 {
				 if(iRetVec[i]!= TC_HashMapMalloc::RT_OK)
					 bOK = false;
				 else if(iRetVec[i]!= TC_HashMapMalloc::RT_OK && iRetVec[i]!=TC_HashMapMalloc::RT_NONEED_SYNC && iRetVec[i]!=TC_HashMapMalloc::RT_ONLY_KEY&&iRetVec[i]!=TC_HashMapMalloc::RT_NEED_SYNC)
					 bError = true;
			 }
			 if( bError)
			 {
				 return TC_HashMapMalloc::RT_EXCEPTION_ERR;
			 }
			 else if(bOK)
			 {
				 return TC_HashMapMalloc::RT_OK;
			 }
			 else
			 {
				 return TC_HashMapMalloc::RT_NEED_SYNC;
			 }
		 }

         int calculateData(int index, uint32_t &count, bool &isEnd)
         {
             return _jceHashMapVec[index]->calculateData(count, isEnd);
         }

        int resetCalculateData(int index)
        {
            return _jceHashMapVec[index]->resetCalculateData();
        }

		 string descDetail() 
		 {
			 string descStr("");
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				descStr += "--------------------------------------------\n";
				descStr += "JMEMNUM: "+TC_Common::tostr(i)+"\n";
				descStr += _jceHashMapVec[i]->desc();
			 }
			 return descStr;
		 }

		 size_t getDataMemSize()
		 {
			 size_t dataMemSize = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				dataMemSize += _jceHashMapVec[i]->getDataMemSize();
			 }
			 return dataMemSize;
		 }
		size_t getMemSize()
		{
			 size_t iMemSize = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				iMemSize += _jceHashMapVec[i]->getMapHead()._iMemSize;
			 }
			 return iMemSize;
		}
		size_t getUsedDataMem()
		{
			 size_t iUsedDataMem = 0;
			 for(size_t i = 0;i<_jmemNum;i++)
			 {
				iUsedDataMem += _jceHashMapVec[i]->getMapHead()._iUsedDataMem;
			 }
			 return iUsedDataMem;
		}
        auto_hash_iterator hashByPos(uint32_t index)
		{
			uint32_t tmpIndex = index;
            for(unsigned int i = 0; i < _jmemNum; i++)
            {
                auto_hash_iterator it(this,this->_jceHashMapVec[i]->hashBegin(),i,_jmemNum);
                uint32_t hashCount = it->getHashCount();
                if(tmpIndex <= (hashCount - 1))
                {
                    if(it.setIndex(tmpIndex) == false)
                        return hashEnd();
                    else
                    {
                        return it;
                    }
                }
                else
                    tmpIndex -= hashCount;
            }
            //到这里表示有问题了
    		return hashEnd();
		}
	    auto_hash_iterator hashBegin()
		{
			return auto_hash_iterator(this,this->_jceHashMapVec[0]->hashBegin(),0,_jmemNum);
		}
		auto_hash_iterator hashEnd()
		{
			return auto_hash_iterator(this,this->_jceHashMapVec[_jmemNum-1]->hashEnd(),_jmemNum-1,_jmemNum);
		}
		int dump2file(const string &sFile)
		{
			TC_LockT<typename LockPolicy::Mutex> lock(_Mutex);
			FILE *fp = fopen(sFile.c_str(), "wb");
			if(fp == NULL)
			{
				return TC_HashMapMalloc::RT_DUMP_FILE_ERR;
			}

			size_t ret = fwrite((void*)_shm.getPointer(), 1, _shm.size(), fp);
			fclose(fp);

			if(ret == _shm.size())
			{
				return TC_HashMapMalloc::RT_OK;
			}
			return TC_HashMapMalloc::RT_DUMP_FILE_ERR;
		}
		int load5file(const string &sFile)
		{
			TC_LockT<typename LockPolicy::Mutex> lock(_Mutex);
			FILE *fp = fopen(sFile.c_str(), "rb");
			if(fp == NULL)
			{
				return TC_HashMapMalloc::RT_LOAL_FILE_ERR;
			}
			fseek(fp, 0L, SEEK_END);
			size_t fs = ftell(fp);
			if(fs != _shm.size())
			{
				fclose(fp);
				return TC_HashMapMalloc::RT_LOAL_FILE_ERR;
			}

			fseek(fp, 0L, SEEK_SET);

			size_t iSize    = 1024*1024*10;
			size_t iLen     = 0;
			char *pBuffer = new char[iSize];
			bool bEof = false;
			while(true)
			{
				int ret = fread(pBuffer, 1, iSize, fp);
				if(ret != (int)iSize)
				{
					if(feof(fp))
					{
						bEof = true;
					}
					else
					{
						delete[] pBuffer;
						return TC_HashMapMalloc::RT_LOAL_FILE_ERR;
					}

				}
				memcpy((char*)_shm.getPointer() + iLen, pBuffer, ret);
				iLen += ret;
				if(bEof)
					break;
			}
			fclose(fp);
			delete[] pBuffer;
			if(iLen == _shm.size())
			{
				return TC_HashMapMalloc::RT_OK;
			}

			return TC_HashMapMalloc::RT_LOAL_FILE_ERR;
		} 
	private:
		vector<JmemHashMap *> _jceHashMapVec;
		//jmem个数
		unsigned int _jmemNum;
		//共享内存
		TC_Shm _shm;
		//信号量集
		typename LockPolicy::Mutex _Mutex;

		NormalHash * _pHash;
	};
}

#endif

