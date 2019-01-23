#include "servant/Auth.h"

namespace taf
{

class AuthProcessor
{
public:
    /**
     * 处理鉴权请求
     * @param conn 请求的来源
     * @param requestData 请求的数据
     * @return true:请求被处理了  false:未处理,是非鉴权包,请业务继续处理 
     */
    static bool processAuth(void* conn, const string& requestData);
                        
private:
    static AuthPrx _authPrx;
};

/**
 * server :默认鉴权逻辑
 */
int processAuthReqHelper(const BasicAuthPackage& pkg, const BasicAuthInfo& info);

/**
 * server :默认鉴权方法
 */
int defaultProcessAuthReq(const string& request, const string& expectObj);

/**
 * client:默认生成鉴权请求方法
 */
string defaultCreateAuthReq(const BasicAuthInfo& info, const string& hashMethod = "sha1");

} // end namespace taf

