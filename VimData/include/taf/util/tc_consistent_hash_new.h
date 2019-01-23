#ifndef __TC_CONSISTENT_HASH_NEW_H_
#define __TC_CONSISTENT_HASH_NEW_H_

#include "util/tc_md5.h"
#include "util/tc_autoptr.h"
#include "util/tc_hash_fun.h"

using namespace taf;

namespace taf
{

struct node_T_new
{
	/**
	 *�ڵ�hashֵ
	 */
	long iHashCode;

	/**
	 *�ڵ��±�
	 */
	unsigned int iIndex;
};

enum TC_HashAlgorithmType
{
    E_TC_CONHASH_KETAMAHASH = 0,
    E_TC_CONHASH_DEFAULTHASH = 1
};

/**
 *  @brief hash �㷨�����
 */
class TC_HashAlgorithm : public TC_HandleBase
{
public:
    virtual long hash(const string & sKey) = 0;
    virtual TC_HashAlgorithmType getHashType() = 0;

protected:
    long subTo32Bit(long hash)
    {
        return (hash & 0xFFFFFFFFL);
    }

};

typedef TC_AutoPtr<TC_HashAlgorithm> TC_HashAlgorithmPtr;

/**
 *  @brief ketama hash �㷨��hash�㷨�ο�memcached client
 */
class TC_KetamaHashAlg : public TC_HashAlgorithm
{
public:
    virtual long hash(const string & sKey)
    {
        string sMd5 = TC_MD5::md5bin(sKey);
        const char *p = (const char *) sMd5.c_str();

        long hash = ((long)(p[3] & 0xFF) << 24)
            | ((long)(p[2] & 0xFF) << 16)
            | ((long)(p[1] & 0xFF) << 8)
            | ((long)(p[0] & 0xFF));

        return subTo32Bit(hash);
    }

    virtual TC_HashAlgorithmType getHashType()
    {
        return E_TC_CONHASH_KETAMAHASH;
    }
};

/**
 *  @brief Ĭ�ϵ� hash �㷨
 */
class TC_DefaultHashAlg : public TC_HashAlgorithm
{
public:
    virtual long hash(const string & sKey)
    {
        string sMd5 = TC_MD5::md5bin(sKey);
        const char *p = (const char *) sMd5.c_str();

        long hash = (*(int*)(p)) ^ (*(int*)(p+4)) ^ (*(int*)(p+8)) ^ (*(int*)(p+12));

        return subTo32Bit(hash);
    }

    virtual TC_HashAlgorithmType getHashType()
    {
        return E_TC_CONHASH_DEFAULTHASH;
    }
};

/**
 *  @brief hash alg ����
 */
class TC_HashAlgFactory
{
public:
    static TC_HashAlgorithm *getHashAlg()
    {
        TC_HashAlgorithm *ptrHashAlg = new TC_DefaultHashAlg();

        return ptrHashAlg;
    }

    static TC_HashAlgorithm *getHashAlg(TC_HashAlgorithmType hashType)
    {
        TC_HashAlgorithm *ptrHashAlg = NULL;

        switch(hashType)
        {
            case E_TC_CONHASH_KETAMAHASH:
            {
                ptrHashAlg = new TC_KetamaHashAlg();
                break;
            }
            case E_TC_CONHASH_DEFAULTHASH:
            default:
            {
                ptrHashAlg = new TC_DefaultHashAlg();
                break;
            }
        }

        return ptrHashAlg;
    }
};

/**
 *  @brief һ����hash�㷨��
 */
class  TC_ConsistentHashNew
{
public:

    /**
     *  @brief ���캯��
     */
    TC_ConsistentHashNew()
    {
        _ptrHashAlg = TC_HashAlgFactory::getHashAlg();
    }

    /**
     *  @brief ���캯��
     */
    TC_ConsistentHashNew(TC_HashAlgorithmType hashType)
    {
        _ptrHashAlg = TC_HashAlgFactory::getHashAlg(hashType);
    }

    /**
     * @brief �ڵ�Ƚ�.
     *
     * @param m1 node_T_new���͵Ķ��󣬱ȽϽڵ�֮һ
     * @param m2 node_T_new���͵Ķ��󣬱ȽϽڵ�֮һ
     * @return less or not �ȽϽ����less����ture�����򷵻�false
     */
    static bool less_hash(const node_T_new & m1, const node_T_new & m2)
    {
        return m1.iHashCode < m2.iHashCode;
    }

    /**
     * @brief ���ӽڵ�.
     *
     * @param node  �ڵ�����
     * @param index �ڵ���±�ֵ
     * @return      �ڵ��hashֵ
     */
    int sortNode()
    {
        sort(_vHashList.begin(), _vHashList.end(), less_hash);

        return 0;
    }

    /**
     * @brief ��ӡ�ڵ���Ϣ
     *
     */
    void printNode()
    {
        map<unsigned int, unsigned int> mapNode;
        size_t size = _vHashList.size();

        for (size_t i = 0; i < size; i++)
        {
            if (i == 0)
            {
                unsigned int value = 0xFFFFFFFF - _vHashList[size - 1].iHashCode + _vHashList[0].iHashCode;
                mapNode[_vHashList[0].iIndex] = value;
            }
            else
            {
                unsigned int value = _vHashList[i].iHashCode - _vHashList[i - 1].iHashCode;

                if (mapNode.find(_vHashList[i].iIndex) != mapNode.end())
                {
                    value += mapNode[_vHashList[i].iIndex];
                }

                mapNode[_vHashList[i].iIndex] = value;
            }

            cout << "printNode: " << _vHashList[i].iHashCode << "|" << _vHashList[i].iIndex << "|" << mapNode[_vHashList[i].iIndex] << endl;
        }

        map<unsigned int, unsigned int>::iterator it = mapNode.begin();
        double avg = 100;
        double sum = 0;

        while (it != mapNode.end())
        {
            double tmp = it->second;
            cerr << "result: " << it->first << "|" << it->second << "|" << (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg) << endl;
            sum += (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg) * (tmp * 100 * mapNode.size() / 0xFFFFFFFF - avg);
            it++;
        }

        cerr << "variance: " << sum / mapNode.size() << ", size: " << _vHashList.size() << endl;
    }

    /**
     * @brief ���ӽڵ�.
     *
     * @param node  �ڵ�����
     * @param index �ڵ���±�ֵ
     * @param weight �ڵ��Ȩ�أ�Ĭ��Ϊ1
     * @return      �Ƿ�ɹ�
     */
    int addNode(const string & node, unsigned int index, int weight = 1)
    {
        if (_ptrHashAlg.get() == NULL)
        {
            return -1;
        }

        node_T_new stItem;
        stItem.iIndex = index;

        for (int j = 0; j < weight; j++)
        {
            string virtualNode = node + "_" + TC_Common::tostr<int>(j);

            // TODO: Ŀǰд��2 ��hash �㷨�����Ը�����Ҫѡ��һ�֣�
            // TODO: ����KEMATA Ϊ�ο�memcached client ��hash �㷨��default Ϊԭ�е�hash �㷨�����Խ����ڱ������
            if (_ptrHashAlg->getHashType() == E_TC_CONHASH_KETAMAHASH)
            {
                string sMd5 = TC_MD5::md5bin(virtualNode);
                char *p = (char *) sMd5.c_str();

                for (int i = 0; i < 4; i++)
                {
                    stItem.iHashCode = ((long)(p[i * 4 + 3] & 0xFF) << 24)
                        | ((long)(p[i * 4 + 2] & 0xFF) << 16)
                        | ((long)(p[i * 4 + 1] & 0xFF) << 8)
                        | ((long)(p[i * 4 + 0] & 0xFF));
                    stItem.iIndex = index;
                    _vHashList.push_back(stItem);
                }
            }
            else
            {
                stItem.iHashCode = _ptrHashAlg->hash(virtualNode);
                _vHashList.push_back(stItem);
            }
        }

        return 0;
    }

    /**
     * @brief ��ȡĳkey��Ӧ���Ľڵ�node���±�.
     *
     * @param key      key����
     * @param iIndex  ��Ӧ���Ľڵ��±�
     * @return        0:��ȡ�ɹ�   -1:û�б���ӵĽڵ�
     */
    int getIndex(const string & key, unsigned int & iIndex)
    {
        if(_ptrHashAlg.get() == NULL || _vHashList.size() == 0)
        {
            iIndex = 0;
            return -1;
        }

        long iCode = _ptrHashAlg->hash(TC_MD5::md5bin(key));

        return getIndex(iCode, iIndex);
    }

    /**
     * @brief ��ȡĳhashcode��Ӧ���Ľڵ�node���±�.
     *
     * @param hashcode      hashcode
     * @param iIndex  ��Ӧ���Ľڵ��±�
     * @return        0:��ȡ�ɹ�   -1:û�б���ӵĽڵ�
     */
    int getIndex(long hashcode, unsigned int & iIndex)
    {
        if(_ptrHashAlg.get() == NULL || _vHashList.size() == 0)
        {
            iIndex = 0;
            return -1;
        }

        // ֻ����32λ
        long iCode = (hashcode & 0xFFFFFFFFL);

        int low = 0;
        int high = _vHashList.size();

        if(iCode <= _vHashList[0].iHashCode || iCode > _vHashList[high-1].iHashCode)
        {
            iIndex = _vHashList[0].iIndex;
            return 0;
        }

        while (low < high - 1)
        {
            int mid = (low + high) / 2;
            if (_vHashList[mid].iHashCode > iCode)
            {
                high = mid;
            }
            else
            {
                low = mid;
            }
        }
        iIndex = _vHashList[low+1].iIndex;
        return 0;
    }

    /**
     * @brief ��ȡ��ǰhash�б�ĳ���.
     *
     * @return        ����ֵ
     */
    size_t size()
    {
        return _vHashList.size();
    }

    /**
     * @brief ��յ�ǰ��hash�б�.
     *
     */
    void clear()
    {
        _vHashList.clear();
    }

protected:
    vector<node_T_new>	_vHashList;
    TC_HashAlgorithmPtr _ptrHashAlg;

};

}
#endif
