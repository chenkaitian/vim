#ifndef __TC_READERS_WRITER_DATA_H__
#define __TC_READERS_WRITER_DATA_H__


namespace taf
{

/**
 * 常用的多读一写数据的封装
 * 常用于数据量比较小的字典数据、配置数据
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

    //获取读取的数据
    virtual T& getReaderData() {
    	if(reader_using_inst0_) {
    		return inst0_;
    	}
    	else {
    		return inst1_;
    	}
    }

    //获取可写的数据
    virtual T& getWriterData() {
    	if(reader_using_inst0_) {
    		return inst1_;
    	}
    	else {
    		return inst0_;
    	}
    }

    //写完数据以后，将读写数据交换
    virtual void swap() {
    	reader_using_inst0_ = !reader_using_inst0_;
    }
};
}
#endif
