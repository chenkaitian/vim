#ifndef __TC_READERS_WRITER_DATA_H__
#define __TC_READERS_WRITER_DATA_H__


namespace taf
{

/**
 * ���õĶ��һд���ݵķ�װ
 * �������������Ƚ�С���ֵ����ݡ���������
 */
template<typename T>
class TC_ReadersWriterData {
private:
	T inst0_;
	T inst1_;
    bool reader_using_inst0_;
public:
    TC_ReadersWriterData() :
    	reader_using_inst0_(true) {
	}

    virtual ~TC_ReadersWriterData() {
    }

    //��ȡ��ȡ������
    virtual T& getReaderData() {
    	if(reader_using_inst0_) {
    		return inst0_;
    	}
    	else {
    		return inst1_;
    	}
    }

    //��ȡ��д������
    virtual T& getWriterData() {
    	if(reader_using_inst0_) {
    		return inst1_;
    	}
    	else {
    		return inst0_;
    	}
    }

    //д�������Ժ󣬽���д���ݽ���
    virtual void swap() {
    	reader_using_inst0_ = !reader_using_inst0_;
    }
};
}
#endif
