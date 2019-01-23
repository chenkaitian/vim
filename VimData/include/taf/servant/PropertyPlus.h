#ifndef __TAF_PROPERTY_PLUS_H_
#define __TAF_PROPERTY_PLUS_H_

#include "servant/PropertyPlusF.h"
#include "util/tc_monitor.h"


#define LANG   					"taf"
#define REPORT_NAME      		"pp_taf.tafstat_api"
#define PP_VERSION  			"1.01"

using namespace std;
using namespace taf;
using namespace LogStat;

namespace taf
{

class PropertyPlus:  public TC_ThreadMutex , public TC_HandleBase
{
public:
	PropertyPlus();
	~PropertyPlus();

	enum POLICY{SUM=0,AVG=1,MAX=2,MIN=3};

	void report(const vector<string> &vKeys, const vector<float> &vValues);

	void report(const vector<string> &vKeys, const vector<pair<float,POLICY> > &vValues);

	void doReport(const string &sPropertyName, LogStat::StatLog &data);

	void doInnerStat(const string &sPropertyName, LogStat::StatLog &data);

private:
	void doMerge(const vector<string> &vKeys, const vector< pair<float,POLICY> > &vValues);

	void doMerge(const vector<string> &vKeys, const vector<float> &vValues);



private:
	template <typename T>
	string toString(T &v)
	{
		taf::JceOutputStream<BufferWriter> os;
		v.writeTo(os);
		return string(os.getBuffer(),os.getLength());
	}

	string toSkey(const vector<string> &v)
	{
		taf::JceOutputStream < BufferWriter > os;
		os.write(v, 0);
		return string(os.getBuffer(), os.getLength());
	}

	void   toVkey(const string &str, vector<string> &v)
	{
		taf::JceInputStream < BufferReader > is;
		is.setBuffer(str.c_str(), str.length());
		is.read(v, 0, true);
	}

private:
	string _sServerName;
	string _sLocalIP;

	map<POLICY, string> _mPolicy;

	unsigned int _iNeedReport;			//需要上报数
	unsigned int _iActualNUM;			//实际上报数
	unsigned int _iLostNum;				//不符合上报规范数
	unsigned int _iExError;				 //异常错误数

	map<string, vector<LogStat::StatValue>  > _mReportData;

};

typedef TC_AutoPtr<PropertyPlus> PropertyPlusPtr;
}
#endif
