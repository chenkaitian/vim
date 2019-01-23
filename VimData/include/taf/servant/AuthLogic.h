#include "servant/Auth.h"

namespace taf
{

class AuthProcessor
{
public:
    /**
     * �����Ȩ����
     * @param conn �������Դ
     * @param requestData ���������
     * @return true:���󱻴�����  false:δ����,�ǷǼ�Ȩ��,��ҵ��������� 
     */
    static bool processAuth(void* conn, const string& requestData);
                        
private:
    static AuthPrx _authPrx;
};

/**
 * server :Ĭ�ϼ�Ȩ�߼�
 */
int processAuthReqHelper(const BasicAuthPackage& pkg, const BasicAuthInfo& info);

/**
 * server :Ĭ�ϼ�Ȩ����
 */
int defaultProcessAuthReq(const string& request, const string& expectObj);

/**
 * client:Ĭ�����ɼ�Ȩ���󷽷�
 */
string defaultCreateAuthReq(const BasicAuthInfo& info, const string& hashMethod = "sha1");

} // end namespace taf

