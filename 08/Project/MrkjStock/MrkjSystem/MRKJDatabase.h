#pragma once

// ################################################################################
// # ADO ���ݿ�֧��
#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
        no_namespace rename("EOF", "adoEOF")


#include "DayDataEntry.h"

// ��ȡ������:Ϊ�˵���ʱ,���Է����������ݿ�
inline const char *GetHostName()
{
    AfxSocketInit();
    static char buf[MAX_PATH] = { 0 };
    int err = gethostname(buf, sizeof(buf));
    int code = WSAGetLastError();
    if(0 == err) {
        return  buf;
    }
    else {
        return "AGAIN";
    }
}

class CMRKJDatabase
{
public:
    static CMRKJDatabase &Instance()
    {
        static CMRKJDatabase _instance;
        return _instance;
    }
    virtual ~CMRKJDatabase();
    // �������ݿ�
    bool Connect(const char *host = GetHostName(), const char *database = "MrkjStock", const char *username = "sa", const char *password = "sql2012");
    // �ر����ݿ�
    void Close();

    // ��ѯ���ݿ��еı� strCode ���� [1,7) ��һ���������� ��Ʊ����,����Խ��,ƥ��Ĺ�Ʊ��Խ��
    bool QueryDayDataTables(std::vector<std::wstring> &vTableNames, const std::wstring &strCode);

    // ȷ�� �����:���������,Ҫ�������ݱ�,���ŵ� code_table����
    // �������,����¹�Ʊ��������
    bool MakeSureExitsTable(const wchar_t *szCode, const wchar_t *szName);

    // ��������ڣ��򴴽� �������ݱ�
    bool MakeDayTable(const std::wstring &strStockCode);

    // ���ӹ�Ʊ����,���Ƶ����ݿ�
    bool AddStock(const std::wstring &strStockCode, const std::wstring &strStockName);
    bool AddStock(const std::vector<std::pair<std::wstring, std::wstring>> &v);

    // ���� ��Ʊ��Ϣ
    bool AddStockInfo(const wchar_t *code, const wchar_t *name, int nKindId);

    // ���¹�Ʊ����
    bool UpdateStockNameByCode(const wchar_t *szCode, const wchar_t *szNewName);

    // ����,����, ����ID, ������;
    typedef std::tuple<std::wstring, std::wstring, int, std::wstring> TupleStockInfo;

    // ��ѯtuple<��Ʊ����,����,����ID,������>
    bool QueryStockInfo(std::vector<TupleStockInfo> &v);
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<std::wstring> vCodes);
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<int> vKinds);

    // ��ָָ��ҳ����
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, int iPage, int countOnePage);

    bool QueryStockInfo(std::vector<TupleStockInfo> &v, int kindId);
    // ģ����ѯ
    bool QueryStockInfoFuzzy(std::vector<TupleStockInfo> &v, const wchar_t *szInput);
    // ��ѯ��Ʊ��Ϣ, ���ݹ�Ʊ����
    bool QueryStockInfoByCode(TupleStockInfo &info, const wchar_t *szCode);
    bool QueryStockInfoByName(TupleStockInfo &info, const wchar_t *name);
    // ��ѯ��Ʊ����, ���ݹ�Ʊ����
    bool QueryStockNameByCode(const wchar_t *code, std::wstring &name);
    // �����¹�Ʊ��Ϣ�������������ݱ�, ���������������,�͸���
    bool UpdStockInfoOrAddTableByStockCode(const TupleStockInfo &info);
    // ���¹�Ʊ��Ϣ
    bool UpdateStockInfoByStockCode(const TupleStockInfo &info);
    // ȷ����Ϣ�Ƿ����
    bool IsExistsStockInfo(const wchar_t *szStockCode);
    // ɾ����Ʊ��Ϣ
    bool DelStockInfoByCode(const wchar_t *szStockCode);


    /*    �������ݱ�    */
    // ��������
    bool AddDayData(const CDayDataEntry &data);
    bool AddDayData(const VDayDataEntry &v);

    // �ж�'����'�����Ƿ����
    bool IsExistDayData(const wchar_t *szCode, const wchar_t *szDate);
    // ɾ������: ɾ��ĳ֧��Ʊָ�����ڵ�����
    bool DelDayData(const std::wstring &strStockCode, const std::wstring &date);
    // �޸�����: ����  data.code and data.date
    bool UpdateDayData(const CDayDataEntry &data);
    // ����֮ǰ�����ڽ����޸�
    bool UpdateDayData(const CDayDataEntry &data, const wchar_t *oldDate);
    // ����������ݴ���,����,�������������
    bool AddOrUpdateDayDataByDate(const CDayDataEntry &data);

    // ��ѯ������
    // v : ���ؽ��
    // szStockCode : ��Ʊ����
    // szDateStart : ��ѯ����֮��ʼ����, ���Ϊnullptr���ʾ�����ƿ�ʼ����
    // szDateEnd : ��ѯ����֮��������, ���Ϊnullptr���ʾ�����ƽ�������
    bool QueryDayData(VDayDataEntry &v, const wchar_t *szStockCode, const wchar_t *szDateStart = nullptr, const wchar_t *szDataEnd = nullptr);

    /* ��� */
    // ��ѯ����
    bool QueryKind(std::vector<std::tuple<int, std::wstring>> &v);
    // �жϰ���Ƿ����
    bool IsExistsKindName(const wchar_t *szName);
    // ��ѯ  ����id
    bool QueryKindIdByName(int &kind_id, const wchar_t *szKindName);
    // ���Ӱ��
    bool AddKind(const wchar_t *szName);
    // ɾ�����
    bool DelKind(const wchar_t *szName);
    // ���°����
    bool UpdateKind(int id, const wchar_t *szName);

    /*********************************************************************************/
    // �û���Ȩ�޹���
    typedef struct {
        std::wstring id;
        std::wstring username;
        std::wstring password;
        std::wstring p001; // ϵͳȨ��
        std::wstring p002; // ��������ά��Ȩ��
        std::wstring p003; // Ʒ��ά��Ȩ��
        std::wstring p004; // ���뵼��Ȩ��
        std::wstring p005; // ��ѯȨ��
        std::wstring p006; // ��ʱ����Ȩ��
        std::wstring p007; // ����Ȩ��
        std::wstring p008; // ���ڼ�����Ȩ��
        std::wstring p009; // ��Ļ��ͼȨ��
        std::wstring p010; // ϵͳ����Ȩ��

    } DBEntryUser, *PDBEntryUser;
    typedef vector<DBEntryUser> VDBEntryUser;

    // �����û�
    bool AddUser(const DBEntryUser &user);
    // ɾ���û�
    bool DelUser(const wchar_t *szID);
    // �����û�
    bool UpdUser(const DBEntryUser &user);
    // ��ѯ�û�
    bool QueryUser(VDBEntryUser &v);
    bool QueryUser(DBEntryUser &u, const wchar_t *szID);
    bool QueryUserByUsername(DBEntryUser &u, const wchar_t *szUsername);
    // ����û�������,�Ƿ�ƥ��
    bool CheckUser(const wchar_t *szUsername, const wchar_t *szPassword);
    // �û����Ƿ����
    bool IsExistUsername(const wchar_t *szUsername);
private:
    CMRKJDatabase();

    static const std::wstring DAY_NAME_PREFIX;
    //��ñ���
    std::wstring GetDayTableName(const std::wstring &code)
    {
        return DAY_NAME_PREFIX + code;
    }

    //ָ��Connection�����ָ��:
    _ConnectionPtr m_pConnection;
};
// ȫ��Ψһ ���ݿ��������
#define DB (CMRKJDatabase::Instance())
