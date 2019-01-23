/*
 * FZPropertyPlusStatManager.h
 *
 *  Created on: 2016年10月9日
 *      Author: ravizhang
 */

#ifndef PROPERTY_PLUS_MAN_H_
#define PROPERTY_PLUS_MAN_H_

#include "servant/Application.h"
#include <servant/Communicator.h>
#include <servant/CommunicatorFactory.h>
#include "promise/promise.h"
#include "servant/PropertyReport.h"



#define API_STAT_APP_SERVER_NAME "SendApi.sendapi"
#define API_STAT_BUSS_APP_SVR taf::ServerConfig::Application + "." + taf::ServerConfig::ServerName

const int PPP_INIT_OK = 0;
const int PPP_INIT_ERR = -1;
const int PPP_REPORT_OK = 0;

#define DEFINE_GLOBAL_PP_PPP(ppname)  PropertyPlusPtr _fz_ng_##ppname; 

#define INIT_PP_PPP(ppname,statname,pptype) do {  FZPropertyPlusStatManager::getInstance()->initPPStat(_fz_ng_##ppname,#statname);}while(0)

#define DEFINE_GLOBAL_MVPP(ppname) PropertyPlusPtr _fz_ng_##ppname;
#define INIT_MV_PP(ppname,statname) do {FZPropertyPlusStatManager::getInstance()->initPPStat(_fz_ng_##ppname,"fz_"#statname);}while(0)


#define PPCOUNT PropertyReport::count()
#define PPSUM PropertyReport::sum()
#define PPAVG PropertyReport::avg()
#define PPDISTR PropertyReport::distr()
#define PPMMA PropertyReport::avg(),PropertyReport::max(),PropertyReport::min()
#define PPMMAS PropertyReport::avg(),PropertyReport::max(),PropertyReport::min(),PropertyReport::sum()

#define RPT_MAX PropertyPlus::MAX
#define RPT_SUM PropertyPlus::SUM
#define RPT_AVG PropertyPlus::AVG
#define RPT_MIN PropertyPlus::MIN

#define PP_REPORT1(ppname,value,ppptype) do {FZPropertyPlusStatManager::getInstance()->ppReport(_fz_ng_##ppname,value,ppptype);}while(0)

#define PP_INIT_KEYS(keys_name) vector<std::string> keys_name;
#define PP_MK_KEY(keys_name,key) do {keys_name.push_back(key);}while(0);


typedef std::vector<pair<float,PropertyPlus::POLICY> > PP_VALUE_VEC;

#define PP_INIT_VALUES(values_name) PP_VALUE_VEC values_name;
#define PP_MK_VALUE(values_name,value,policy) do {pair<float,PropertyPlus::POLICY>_inner_tmp_pair; \
        _inner_tmp_pair.first = value;\
        _inner_tmp_pair.second = policy;\
        values_name.push_back(_inner_tmp_pair);}while(0);
        
#define PP_REPORT_MKV(ppname,keys,values) do {PP_MK_KEY(keys,FZConfigManager::getInstance()->getInstID());\
    FZPropertyPlusStatManager::getInstance()->ppReportKV(_fz_ng_##ppname,keys,values);} while(0);

class FZPropertyPlusStatManager :
		public TC_Singleton<FZPropertyPlusStatManager, CreateUsingNew, DefaultLifetime>
{
public:
	int initPPStat(PropertyPlusPtr & ppp , std::string ppName);
	//no other keys , one value# 
	int ppReport(PropertyPlusPtr ppp , float v1 , PropertyPlus::POLICY policy1); 
	int ppReportKV(PropertyPlusPtr ppp , const vector<std::string> & keys  , const PP_VALUE_VEC & values); 	
};


#endif
