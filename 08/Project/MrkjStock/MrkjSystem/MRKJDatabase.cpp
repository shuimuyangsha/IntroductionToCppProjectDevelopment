#include "stdafx.h"
#include "MRKJDatabase.h"
#include "DayDataEntry.h"
#include "TableFiles.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>

using namespace std;

const std::wstring CMRKJDatabase::DAY_NAME_PREFIX = L"T_R_";




CMRKJDatabase::CMRKJDatabase()
{
    // ����һ������ָ��
    if(S_OK != m_pConnection.CreateInstance(__uuidof(Connection))) {
        assert(false);
    }
    // �������ӳ�ʱʱ��=5��
    m_pConnection->ConnectionTimeout = 5;
}


CMRKJDatabase::~CMRKJDatabase()
{
}

bool CMRKJDatabase::Connect(const char *host/*=nullptr*/
                            , const char *database/*=nullptr*/
                            , const char *username/*=nullptr*/
                            , const char *password/*=nullptr*/)
{
    using namespace std;
    try {
        // �����ݿ�
        std::string connectionString = "driver={SQL Server};Server=" + string(host) +
                                       ";Database=" + string(database) + ";UID=" +
                                       string(username) + ";PWD=" +
                                       string(password) + ";";
        HRESULT hr = m_pConnection->Open(connectionString.c_str(), username, password
                                         , adModeUnknown);
        assert(SUCCEEDED(hr));
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

void CMRKJDatabase::Close()
{
    if(m_pConnection->State) {
        m_pConnection->Close();
    }
    m_pConnection.Release();
    m_pConnection = nullptr;
}

// ��ѯ���ݿ��еı�
bool CMRKJDatabase::QueryDayDataTables(std::vector<std::wstring> &v, const std::wstring &strCode)
{
    // �����Ʊ���볤�� ���� [1,6] ֮��
    if(!(strCode.length() >= 1 && strCode.length() < 7)) {
        return false;
    }
    const std::wstring NAME(L"name");
    std::wstring sql =
        L"SELECT " + NAME + " FROM sys.objects \r\n"
        L"    WHERE type = 'U' \r\n"
        L"    AND type_desc = 'USER_TABLE' \r\n"
        L"    AND name like '" + DAY_NAME_PREFIX;
    sql += strCode;
    sql += L"%';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t name = m_pRecordset->GetCollect(NAME.c_str());
            assert(name.vt == VT_BSTR);
            std::wstring prefixName = name.bstrVal;
            v.push_back(prefixName.substr(DAY_NAME_PREFIX.length(), prefixName.length() - DAY_NAME_PREFIX.length()));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
    return true;
}

// ��ѯtuple<��Ʊ����,����,����ID,������>
bool CMRKJDatabase::QueryStockInfo(std::vector<std::tuple<std::wstring, std::wstring, int, std::wstring>> &v)
{
    std::wstring sql =
        L"SELECT \r\n"
        L"    TS.CODE, \r\n"
        L"    TS.NAME, \r\n"
        L"    ISNULL(TS.ID_KIND, 0) KIND_ID, \r\n"
        L"    ISNULL(TK.NAME, '') KIND_NAME \r\n"
        L"FROM T_STOCK_INFO TS LEFT JOIN T_KIND TK \r\n"
        L"    ON TS.ID_KIND = TK.ID \r\n";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            _variant_t id = m_pRecordset->GetCollect(L"KIND_ID");
            _variant_t kindname = m_pRecordset->GetCollect(L"KIND_NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, id.intVal, kindname.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<std::wstring> vCodes)
{
    std::wstring sql =
        L"SELECT \r\n"
        L"    TS.CODE, \r\n"
        L"    TS.NAME, \r\n"
        L"    ISNULL(TS.ID_KIND, 0) KIND_ID, \r\n"
        L"    ISNULL(TK.NAME, '') KIND_NAME \r\n"
        L"FROM T_STOCK_INFO TS LEFT JOIN T_KIND TK \r\n"
        L"    ON TS.ID_KIND = TK.ID \r\n"
        L"WHERE TS.CODE IN ("
        ;
    std::wstring cond = std::accumulate(vCodes.begin()
                                        , vCodes.end()
                                        , std::wstring{}
    , [](std::wstring & a, std::wstring & b)->std::wstring {
        if(!a.empty())
        {
            return a + L",'" + b + L"'";
        }
        else {
            return L"'" + b + L"'";
        }
    });
    cond.append(L");\r\n");
    sql += cond;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            _variant_t id = m_pRecordset->GetCollect(L"KIND_ID");
            _variant_t kindname = m_pRecordset->GetCollect(L"KIND_NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, id.intVal, kindname.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;

}

bool CMRKJDatabase::QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<int> vKinds)
{
    std::wstring sql =
        L"SELECT \r\n"
        L"    TS.CODE, \r\n"
        L"    TS.NAME, \r\n"
        L"    TS.ID_KIND KIND_ID, \r\n"
        L"    ISNULL(TK.NAME, '') KIND_NAME \r\n"
        L"FROM T_STOCK_INFO TS, \r\n"
        L"     T_KIND TK \r\n"
        L"WHERE  TS.ID_KIND = TK.ID \r\n"
        L"     AND TK.ID IN (";

    std::wstring cond = std::accumulate(vKinds.begin()
                                        , vKinds.end()
                                        , std::wstring{}
    , [](std::wstring & a, int b)->std::wstring {
        std::wstringstream ss;
        ss << b;
        if(!a.empty())
        {
            return a + L"," + ss.str() + L"";
        }
        else {
            return L"" + ss.str() + L"";
        }
    });
    cond.append(L");\r\n");
    sql += cond;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            _variant_t id = m_pRecordset->GetCollect(L"KIND_ID");
            _variant_t kindname = m_pRecordset->GetCollect(L"KIND_NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, id.intVal, kindname.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;

}



// ��ָָ��ҳ����
bool CMRKJDatabase::QueryStockInfo(std::vector<TupleStockInfo> &v, int iPage, int countOnePage)
{
    WCHAR buf[1024] = {0};
    wsprintfW(buf,
              L" WITH T_ROW_NUMBER AS( \r\n"
              L"     SELECT ROW_NUMBER() OVER(ORDER BY CODE ASC) as RN, *FROM T_STOCK_INFO WHERE 1 = 1 \r\n"
              L" ) \r\n"
              L" SELECT \r\n"
              L"     TS.CODE CODE, \r\n"
              L"     TS.NAME NAME, \r\n"
              L"     ISNULL(TS.ID_KIND, 0) KIND_ID, \r\n"
              L"     ISNULL(TK.NAME, '') KIND_NAME \r\n"
              L" FROM T_ROW_NUMBER TS \r\n"
              L"     LEFT JOIN T_KIND TK \r\n"
              L"     ON TS.ID_KIND = TK.ID \r\n"
              L" WHERE RN >= %d AND RN <= %d  \r\n"
              , (iPage - 1) * countOnePage, iPage * countOnePage);
    std::wstring sql = buf;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            _variant_t id = m_pRecordset->GetCollect(L"KIND_ID");
            _variant_t kindname = m_pRecordset->GetCollect(L"KIND_NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, id.intVal, kindname.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;

}

bool CMRKJDatabase::QueryStockInfo(std::vector<TupleStockInfo> &v, int kindId)
{
    wchar_t szBuf[1024] = { 0 };
    _stprintf_s(szBuf,
                L"SELECT \r\n"
                L"    TS.CODE, \r\n"
                L"    TS.NAME, \r\n"
                L"    ISNULL(TS.ID_KIND, 0) KIND_ID, \r\n"
                L"    ISNULL(TK.NAME, '') KIND_NAME \r\n"
                L"FROM T_STOCK_INFO TS LEFT JOIN T_KIND TK \r\n"
                L"    ON TS.ID_KIND = TK.ID \r\n"
                L"WHERE TS.ID_KIND = %d \r\n",
                kindId);
    std::wstring sql = szBuf;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            _variant_t id = m_pRecordset->GetCollect(L"KIND_ID");
            _variant_t kindname = m_pRecordset->GetCollect(L"KIND_NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, id.intVal, kindname.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ģ����ѯ
bool CMRKJDatabase::QueryStockInfoFuzzy(std::vector<TupleStockInfo> &v, const wchar_t *szInput)
{
    wchar_t sqSql[1024] = { 0 };
    _stprintf_s(sqSql,
                L" SELECT \r\n"
                L"     TS.CODE, \r\n"
                L"     TS.NAME \r\n"
                L" FROM T_STOCK_INFO TS\r\n"
                L" WHERE TS.CODE LIKE '%%%s%%' \r\n"
                L" OR TS.NAME LIKE '%%%s%%'; \r\n", szInput, szInput);
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sqSql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            _variant_t code = m_pRecordset->GetCollect(L"CODE");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            v.push_back(std::make_tuple(code.bstrVal, name.bstrVal, 0, L""));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ȷ�� �����:���������,Ҫ�������ݱ�,���ŵ� code_table����
bool CMRKJDatabase::MakeSureExitsTable(const wchar_t *szCode, const wchar_t *szName)
{
    // ��ʼ����
    m_pConnection->BeginTrans();
    bool b = false;
    std::wstring name;
    b = QueryStockNameByCode(szCode, name);
    // �������û��,�����¼�¼
    if(!b) {
        if(!AddStock(szCode, szName)) {
            goto __Rollback;
        }
        // ����ɹ�,�������ݱ�
        if(!MakeDayTable(szCode)) {
            goto __Rollback;
        }
        //�ɹ�, Commit;
    }
    // ���������,��������
    else {
        if(!UpdateStockNameByCode(szCode, szName)) {
            goto __Rollback;
        }
        // ����ɹ�,�������ݱ�
        if(!MakeDayTable(szCode)) {
            goto __Rollback;
        }
        //�ɹ�, Commit;
    }
    // ��������
    m_pConnection->CommitTrans();
    return true;
__Rollback:
    m_pConnection->RollbackTrans();
    return false;
}

// ��ѯ��Ʊ��Ϣ, ���ݹ�Ʊ����
bool CMRKJDatabase::QueryStockInfoByCode(TupleStockInfo &info, const wchar_t *szCode)
{
    typedef std::tuple<std::wstring, std::wstring, int, std::wstring> _type;
    vector<_type> v;
    if(!QueryStockInfo(v)) {
        return false;
    }
    // ����
    auto it = std::find_if(v.begin(), v.end(), [szCode](_type & rhs)->bool {
        return std::get<0>(rhs) == szCode;
    });
    if(it == v.end()) {
        return false;
    }
    info = *it;
    return true;
}

bool CMRKJDatabase::QueryStockInfoByName(TupleStockInfo &info, const wchar_t *name)
{
    typedef std::tuple<std::wstring, std::wstring, int, std::wstring> _type;
    vector<_type> v;
    if(!QueryStockInfo(v)) {
        return false;
    }
    // ����
    auto it = std::find_if(v.begin(), v.end(), [name](_type & rhs)->bool {
        return std::get<1>(rhs) == name;
    });
    if(it == v.end()) {
        return false;
    }
    info = *it;
    return true;
}

// ��ѯ��Ʊ����, ���ݹ�Ʊ����
bool CMRKJDatabase::QueryStockNameByCode(const wchar_t *code, std::wstring &name)
{
    assert(code);
    if(_tcslen(code) != TableFiles::STOCK_LEN) {
        return false;
    }
    std::wstring sql = L"SELECT NAME FROM T_STOCK_INFO WHERE CODE ='";
    sql += code;
    sql += L"'";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            _variant_t var_name = m_pRecordset->GetCollect(L"NAME");
            assert(var_name.vt == VT_BSTR);
            name = var_name.bstrVal;
            return true;
        }
        else {
            return false;
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// �����¹�Ʊ��Ϣ�������������ݱ�, ���������������,�͸���
bool CMRKJDatabase::UpdStockInfoOrAddTableByStockCode(const TupleStockInfo &info)
{
    // ��������
    m_pConnection->BeginTrans();
    // ȷ���Ƿ���Ϣ
    bool b = IsExistsStockInfo(std::get<0>(info).c_str());
    // �������Ϣ,����
    if(b) {
        // ������Ϣ
        if(!UpdateStockInfoByStockCode(info)) {
            goto __Rollback;
        }
    }
    // �������,���������ݱ�
    else {
        if(!AddStockInfo(get<0>(info).c_str(), get<1>(info).c_str(), get<2>(info))) {
            goto __Rollback;
        }
        // ����
        if(!MakeDayTable(get<0>(info))) {
            goto __Rollback;
        }
    }
    m_pConnection->CommitTrans();
    return true;
__Rollback:
    m_pConnection->RollbackTrans();
    return false;
}

// ���¹�Ʊ��Ϣ
bool CMRKJDatabase::UpdateStockInfoByStockCode(const TupleStockInfo &info)
{
    using namespace std;
    wstring sql = L"SELECT * FROM T_STOCK_INFO WHERE CODE = '" + get<0>(info) + L"'";
    bool bUpdated = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            // �޸ļ�¼
            m_pRecordset->PutCollect(L"CODE", _variant_t(get<0>(info).c_str()));
            m_pRecordset->PutCollect(L"NAME", _variant_t(get<1>(info).c_str()));
            m_pRecordset->PutCollect(L"ID_KIND", _variant_t(get<2>(info)));
            m_pRecordset->Update();
            bUpdated = true;
        }
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bUpdated;
}

// ȷ����Ϣ�Ƿ����
bool CMRKJDatabase::IsExistsStockInfo(const wchar_t *szStockCode)
{
    wchar_t szSql[1024] = { 0 };
    _stprintf_s(szSql, _T("SELECT CODE FROM  T_STOCK_INFO WHERE CODE = '%s'"), szStockCode);
    bool bFind = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(szSql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            bFind = true;
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}

// ɾ����Ʊ��Ϣ
bool CMRKJDatabase::DelStockInfoByCode(const wchar_t *szStockCode)
{
    using namespace std;
    const wstring sql = L"SELECT * FROM T_STOCK_INFO WHERE CODE = '" + wstring(szStockCode) + L"';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->Delete(adAffectCurrent);
            m_pRecordset->Update();
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ��������ڣ��򴴽�
bool CMRKJDatabase::MakeDayTable(const std::wstring &strStockCode)
{
    using namespace std;
    // data.code + "r" = ����
    const wstring wstrTableName = GetDayTableName(strStockCode);
    TCHAR sql[1024] = { 0 };
    PCTSTR szFmt =
        L"IF NOT EXISTS(SELECT 1 FROM sysobjects WHERE ID = OBJECT_ID('%s'))\r\n"
        L"    BEGIN\r\n"
        L"    CREATE TABLE dbo.%s\r\n"
        L"    (   %s varchar(10) PRIMARY KEY NOT NULL,\r\n"
        L"        %s varchar(32)  NOT NULL,\r\n"
        L"        %s varchar(32)  NOT NULL,\r\n"
        L"        %s varchar(32)  NOT NULL,\r\n"
        L"        %s varchar(32) NOT NULL,\r\n"
        L"        %s varchar(32) NOT NULL,\r\n"
        L"        %s varchar(32) NOT NULL\r\n"
        L"        )\r\n"
        L"    END;\r\n";
    _stprintf_s(sql, szFmt, wstrTableName.c_str(), wstrTableName.c_str(),
                TableFiles::DATA_DATE, TableFiles::PRICE_OPEN, TableFiles::PRICE_CLOSE,
                TableFiles::PRICE_MAX, TableFiles::PRICE_MIN, TableFiles::TURNOVER,
                TableFiles::TRADING_VOLUME);
    try {
        _variant_t RecordsAffected;
        m_pConnection->Execute(sql, &RecordsAffected, adCmdText);
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ���ӹ�Ʊ����,���Ƶ����ݿ�
bool CMRKJDatabase::AddStock(const std::wstring &strStockCode, const std::wstring &strStockName)
{
    using namespace std;
    const wstring sql = L"SELECT * FROM dbo.T_STOCK_INFO;";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        // �ƶ������
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->MoveLast();
        }
        // ���Ӽ�¼
        m_pRecordset->AddNew();
        m_pRecordset->PutCollect(TableFiles::CODE, _variant_t(strStockCode.c_str()));
        m_pRecordset->PutCollect(TableFiles::NAME, _variant_t(strStockName.c_str()));
        m_pRecordset->Update();
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}
bool CMRKJDatabase::AddStock(const std::vector<std::pair<std::wstring, std::wstring>> &v)
{
    using namespace std;
    const wchar_t *szFmt = L"INSERT INTO dbo.T_STOCK_INFO (CODE, NAME) VALUES('%s', '%s');\r\n";
    wchar_t buf[1024] = { 0 };
    try {
        for each(auto pir in v) {
            _stprintf_s(buf, szFmt, pir.first.c_str(), pir.second.c_str());
            _variant_t RecordsAffected;
            m_pConnection->Execute(buf, &RecordsAffected, adCmdText);
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ���� ��Ʊ��Ϣ
bool CMRKJDatabase::AddStockInfo(const wchar_t *code, const wchar_t *name, int nKindId)
{
    using namespace std;
    try {
        const wchar_t *szFmt = L"INSERT INTO dbo.T_STOCK_INFO (CODE, NAME, ID_KIND) VALUES('%s', '%s', %d);\r\n";
        wchar_t buf[1024] = { 0 };
        _stprintf_s(buf, szFmt, code, name, nKindId);
        _variant_t RecordsAffected;
        m_pConnection->Execute(buf, &RecordsAffected, adCmdText);
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}


// ���¹�Ʊ����
bool CMRKJDatabase::UpdateStockNameByCode(const wchar_t *szCode, const wchar_t *szNewName)
{
    using namespace std;
    // data.code + "r" = ����
    wstring sql = L"SELECT NAME FROM ";
    sql += L" T_STOCK_INFO";
    sql += L" WHERE CODE ='";
    sql += szCode;
    sql += L"'";
    bool bUpdated = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            // �޸ļ�¼
            m_pRecordset->PutCollect(TableFiles::NAME, _variant_t(szNewName));
            m_pRecordset->Update();
            bUpdated = true;
        }
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bUpdated;
}

// ��������
bool CMRKJDatabase::AddDayData(const CDayDataEntry &data)
{
    using namespace std;
    assert(!data.code.empty());
    // data.code + "r" = ����
    const wstring wstrTableName = GetDayTableName(data.code);
    // ƴ�� SQL���
    const wstring sql = L"Select * from " + wstrTableName + L";";
    try {
        //����
        _RecordsetPtr rs;
        rs.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        rs->Open(sql.c_str(), m_pConnection.GetInterfacePtr(),
                 adOpenStatic, adLockOptimistic, adCmdText);
        // �ƶ������
        if(!rs->adoEOF) {
            rs->MoveLast();
        }
        // ���Ӽ�¼
        rs->AddNew();
        // ���ø����е�ֵ
        rs->PutCollect(TableFiles::DATA_DATE, _variant_t(data.date.c_str()));
        rs->PutCollect(TableFiles::PRICE_OPEN, _variant_t(data.open.c_str()));
        rs->PutCollect(TableFiles::PRICE_CLOSE, _variant_t(data.close.c_str()));
        rs->PutCollect(TableFiles::PRICE_MAX, _variant_t(data.max.c_str()));
        rs->PutCollect(TableFiles::PRICE_MIN, _variant_t(data.min.c_str()));
        rs->PutCollect(TableFiles::TURNOVER, _variant_t(data.turnover.c_str()));
        rs->PutCollect(TableFiles::TRADING_VOLUME,
                       _variant_t(data.tradingVolume.c_str()));
        // ����
        rs->Update();
        // �رռ�¼��
        rs->Close();
        // �Ѽ�¼������ΪѨ,��ֹ��������
        rs = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::AddDayData(const VDayDataEntry &v)
{
    assert(!v.empty());
    using namespace std;
    // data.code + "r" = ����
    const wstring wstrTableName = GetDayTableName(v[0].code);
    const wstring sql = L"Select * from " + wstrTableName + L";";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic,
                           adLockOptimistic, adCmdText);
        for each(auto data in v) {
            // �ƶ������
            if(!m_pRecordset->adoEOF) {
                m_pRecordset->MoveLast();
            }
            // ���Ӽ�¼
            m_pRecordset->AddNew();
            m_pRecordset->PutCollect(TableFiles::DATA_DATE, _variant_t(data.date.c_str()));
            m_pRecordset->PutCollect(TableFiles::PRICE_OPEN, _variant_t(data.open.c_str()));
            m_pRecordset->PutCollect(TableFiles::PRICE_CLOSE, _variant_t(data.close.c_str()));
            m_pRecordset->PutCollect(TableFiles::PRICE_MAX, _variant_t(data.max.c_str()));
            m_pRecordset->PutCollect(TableFiles::PRICE_MIN, _variant_t(data.min.c_str()));
            m_pRecordset->PutCollect(TableFiles::TURNOVER, _variant_t(data.turnover.c_str()));
            m_pRecordset->PutCollect(TableFiles::TRADING_VOLUME, _variant_t(data.tradingVolume.c_str()));
            m_pRecordset->Update();
        }
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// �ж�'����'�����Ƿ����
bool CMRKJDatabase::IsExistDayData(const wchar_t *szCode, const wchar_t *szDate)
{
    wchar_t szSql[1024] = { 0 };
    _stprintf_s(szSql, _T("SELECT %s FROM %s WHERE %s = '%s'")
                , TableFiles::DATA_DATE
                , GetDayTableName(szCode).c_str()
                , TableFiles::DATA_DATE
                , szDate
               );
    bool bFind = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(szSql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            bFind = true;
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}

// ɾ������: ɾ��ĳ֧��Ʊָ�����ڵ�����
bool CMRKJDatabase::DelDayData(const std::wstring &strStockCode, const std::wstring &date)
{
    using namespace std;
    const wstring wstrTableName = GetDayTableName(strStockCode);
    const wstring sql = L"SELECT * FROM " + wstrTableName +
                        L" WHERE DATA_DATE = '" + date + L"';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(),
                           adOpenStatic, adLockOptimistic, adCmdText);
        // �� ���Ƿ��˽������β��
        if(!m_pRecordset->adoEOF) {
            // ɾ������
            m_pRecordset->Delete(adAffectCurrent);
            // ����,�˴������������ɾ������
            m_pRecordset->Update();
        }
        // �ر�
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// �޸�����
bool CMRKJDatabase::UpdateDayData(const CDayDataEntry &data)
{
    using namespace std;
    assert(!data.code.empty());
    // data.code + "r" = ����
    const wstring wstrTableName = GetDayTableName(data.code);
    const wstring sql = L"SELECT * FROM " + wstrTableName +
                        L" WHERE DATA_DATE ='" + data.date + L"';";
    try {
        //����
        _RecordsetPtr rs;
        rs.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        rs->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic
                 , adLockOptimistic, adCmdText);
        // ��ʱ����ָ����Ŀ���¼������,��ֹ����Ҫ�ƶ�
        // �޸ļ�¼
        rs->PutCollect(TableFiles::DATA_DATE, _variant_t(data.date.c_str()));
        rs->PutCollect(TableFiles::PRICE_OPEN, _variant_t(data.open.c_str()));
        rs->PutCollect(TableFiles::PRICE_CLOSE, _variant_t(data.close.c_str()));
        rs->PutCollect(TableFiles::PRICE_MAX, _variant_t(data.max.c_str()));
        rs->PutCollect(TableFiles::PRICE_MIN, _variant_t(data.min.c_str()));
        rs->PutCollect(TableFiles::TURNOVER, _variant_t(data.turnover.c_str()));
        rs->PutCollect(TableFiles::TRADING_VOLUME,
                       _variant_t(data.tradingVolume.c_str()));
        // ����:�����޸���Ϊ�����ݿ�
        rs->Update();
        // �ر�
        rs->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::UpdateDayData(const CDayDataEntry &data, const wchar_t *oldDate)
{
    using namespace std;
    assert(!data.code.empty());
    // data.code + "r" = ����
    const wstring wstrTableName = GetDayTableName(data.code);
    const wstring sql = L"SELECT * FROM " + wstrTableName + L" WHERE DATA_DATE ='" + wstring(oldDate) + L"';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        // ��ʱ����ָ����Ŀ���¼������,��ֹ����Ҫ�ƶ�
        // �޸ļ�¼
        m_pRecordset->PutCollect(TableFiles::DATA_DATE, _variant_t(data.date.c_str()));
        m_pRecordset->PutCollect(TableFiles::PRICE_OPEN, _variant_t(data.open.c_str()));
        m_pRecordset->PutCollect(TableFiles::PRICE_CLOSE, _variant_t(data.close.c_str()));
        m_pRecordset->PutCollect(TableFiles::PRICE_MAX, _variant_t(data.max.c_str()));
        m_pRecordset->PutCollect(TableFiles::PRICE_MIN, _variant_t(data.min.c_str()));
        m_pRecordset->PutCollect(TableFiles::TURNOVER, _variant_t(data.turnover.c_str()));
        m_pRecordset->PutCollect(TableFiles::TRADING_VOLUME, _variant_t(data.tradingVolume.c_str()));
        m_pRecordset->Update();
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ����������ݴ���,����,�������������
bool CMRKJDatabase::AddOrUpdateDayDataByDate(const CDayDataEntry &data)
{
    if(IsExistDayData(data.code.c_str(), data.date.c_str())) {
        return UpdateDayData(data);
    }
    else {
        return AddDayData(data);
    }
}


// ��������
bool CMRKJDatabase::QueryDayData(VDayDataEntry &v, const wchar_t *szStockCode,
                                 const wchar_t *szDateStart/* = nullptr*/,
                                 const wchar_t *szDataEnd/* = nullptr*/)
{
    // ƴ��SQL���:���������е�����
    std::wstring sql = L"SELECT * FROM ";
    sql += GetDayTableName(szStockCode);
    if(szDateStart && szDataEnd) {
        // ƴ�Ӳ�ѯ����:ָ����ʼ�ͽ�������
        wchar_t sqlbuf[1024] = {};
        // ����������������
        swprintf_s(sqlbuf, _countof(sqlbuf),
                   L" WHERE '%s' <= DATA_DATE and DATA_DATE <= '%s' "
                   L"ORDER BY DATA_DATE;", szDateStart, szDataEnd);
        sql += sqlbuf;
    }
    else if(szDateStart) {
        // ƴ�Ӳ�ѯ����:ָ����ʼ����
        wchar_t sqlbuf[1024] = {};
        // ����������������
        swprintf_s(sqlbuf, _countof(sqlbuf),
                   L" WHERE '%s' <= DATA_DATE ORDER BY DATA_DATE;",
                   szDateStart);
        sql = sql + sqlbuf;
    }
    else if(szDataEnd) {
        // ƴ�Ӳ�ѯ����:ָ����ʼ����
        wchar_t sqlbuf[1024] = {};
        // ����������������
        swprintf_s(sqlbuf, _countof(sqlbuf),
                   L" WHERE DATA_DATE <= '%s' ORDER BY DATA_DATE;",
                   szDataEnd);
        sql = sql + sqlbuf;
    }
    else {
        // û������
    }
    try {
        //����
        _RecordsetPtr rs;
        rs.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        rs->Open(sql.c_str(), m_pConnection.GetInterfacePtr(),
                           adOpenDynamic, adLockOptimistic, adCmdText);
        // ѭ��ֱ�����һ����¼
        while(!rs->adoEOF) {
            CDayDataEntry obj;
            // ȡ�����е�ֵ
            _variant_t date          = rs->GetCollect(TableFiles::DATA_DATE);
            _variant_t open          = rs->GetCollect(TableFiles::PRICE_OPEN);
            _variant_t close         = rs->GetCollect(TableFiles::PRICE_CLOSE);
            _variant_t max           = rs->GetCollect(TableFiles::PRICE_MAX);
            _variant_t min           = rs->GetCollect(TableFiles::PRICE_MIN);
            _variant_t turnover      = rs->GetCollect(TableFiles::TURNOVER);
            _variant_t tradingVolume = rs->GetCollect(TableFiles::TRADING_VOLUME);
            assert(date.vt == VT_BSTR);
            // ��ֵ�������
            obj.code          = szStockCode;
            obj.date          = date.bstrVal;
            obj.open          = open.bstrVal;
            obj.close         = close.bstrVal;
            obj.max           = max.bstrVal;
            obj.min           = min.bstrVal;
            obj.turnover      = turnover.bstrVal;
            obj.tradingVolume = tradingVolume.bstrVal;
            // ���뻺����
            v.push_back(obj);
            // �ƶ�����һ����¼
            rs->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

// ��ѯ���� ���
bool CMRKJDatabase::QueryKind(std::vector<std::tuple<int, std::wstring>> &v)
{
    std::wstring sql = L"SELECT * FROM T_KIND";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            CDayDataEntry obj;
            _variant_t id = m_pRecordset->GetCollect(L"ID");
            _variant_t name = m_pRecordset->GetCollect(L"NAME");
            assert(id.vt == VT_DECIMAL);
            assert(name.vt == VT_BSTR);
            v.push_back(std::make_tuple(id.intVal, name.bstrVal));
            m_pRecordset->MoveNext();
        }
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

//
bool CMRKJDatabase::IsExistsKindName(const wchar_t *szName)
{
    bool bFind = false;
    std::wstring sql = L"SELECT * FROM T_KIND WHERE NAME = '" + std::wstring(szName) + L"'";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}

// ��ѯ  ����id
bool CMRKJDatabase::QueryKindIdByName(int &kind_id, const wchar_t *szKindName)
{
    bool bFind = false;
    std::wstring sql = L"SELECT * FROM T_KIND WHERE NAME = '" + std::wstring(szKindName) + L"'";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            _variant_t id = m_pRecordset->GetCollect(L"ID");
            kind_id = id.intVal;
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}


// ���Ӱ��
bool CMRKJDatabase::AddKind(const wchar_t *szName)
{
    using namespace std;
    int id = -1;
    try {
        const wstring sql_max_id = L"SELECT isnull((SELECT top 1 MAX(ID) MAXID FROM T_KIND), 0) MAXID;";
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql_max_id.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            _variant_t d = m_pRecordset->GetCollect(L"MAXID").intVal;
            assert(d.vt == VT_INT);
            id = d.intVal;
        }
        id++;
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(L"%s", e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    if(id == -1) {
        return false;
    }
    try {
        const wstring sql = L"SELECT ID, NAME FROM T_KIND";
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        // �ƶ������
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->MoveLast();
        }
        // ���Ӽ�¼
        m_pRecordset->AddNew();
        m_pRecordset->PutCollect(L"ID", _variant_t(id));
        m_pRecordset->PutCollect(L"NAME", _variant_t(szName));
        m_pRecordset->Update();
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(L"%s", e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}
bool CMRKJDatabase::DelKind(const wchar_t *szName)
{
    using namespace std;
    const wstring sql = L"SELECT * FROM T_KIND WHERE NAME = '" + std::wstring(szName) + L"';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->Delete(adAffectCurrent);
            m_pRecordset->Update();
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}
bool CMRKJDatabase::UpdateKind(int id, const wchar_t *szName)
{
    using namespace std;
    wchar_t sql[1024] = { 0 };
    _stprintf_s(sql, L"SELECT * FROM T_KIND WHERE ID = %d;", id);
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql, m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        // ��ʱ����ָ����Ŀ���¼������,��ֹ����Ҫ�ƶ�
        if(!m_pRecordset->adoEOF) {
            // �޸ļ�¼
            m_pRecordset->PutCollect(L"NAME", _variant_t(szName));
            m_pRecordset->Update();
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::AddUser(const DBEntryUser &user)
{
    using namespace std;
    const wstring sql = L"SELECT * FROM dbo.T_USER;";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        // �ƶ������
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->MoveLast();
        }
        // ���Ӽ�¼
        m_pRecordset->AddNew();
        m_pRecordset->PutCollect(L"USERNAME", _variant_t(user.username.c_str()));
        m_pRecordset->PutCollect(L"PASSWORD", _variant_t(user.password.c_str()));
        m_pRecordset->PutCollect(L"P001", _variant_t(user.p001.c_str()));
        m_pRecordset->PutCollect(L"P002", _variant_t(user.p002.c_str()));
        m_pRecordset->PutCollect(L"P003", _variant_t(user.p003.c_str()));
        m_pRecordset->PutCollect(L"P004", _variant_t(user.p004.c_str()));
        m_pRecordset->PutCollect(L"P005", _variant_t(user.p005.c_str()));
        m_pRecordset->PutCollect(L"P006", _variant_t(user.p006.c_str()));
        m_pRecordset->PutCollect(L"P007", _variant_t(user.p007.c_str()));
        m_pRecordset->PutCollect(L"P008", _variant_t(user.p008.c_str()));
        m_pRecordset->PutCollect(L"P009", _variant_t(user.p009.c_str()));
        m_pRecordset->PutCollect(L"P010", _variant_t(user.p010.c_str()));
        m_pRecordset->Update();
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::DelUser(const wchar_t *szID)
{
    using namespace std;
    wstring sql = L"SELECT * FROM T_USER WHERE ID = ";
    sql += szID;
    sql += L";";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            m_pRecordset->Delete(adAffectCurrent);
            m_pRecordset->Update();
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}

bool CMRKJDatabase::UpdUser(const DBEntryUser &user)
{
    using namespace std;
    wstring sql = L"SELECT * FROM T_USER WHERE ID = ";
    sql += user.id;
    sql += L";";
    bool bUpdated = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // �򿪼�¼��
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenStatic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            // �޸ļ�¼
            m_pRecordset->PutCollect(L"USERNAME", _variant_t(user.username.c_str()));
            m_pRecordset->PutCollect(L"PASSWORD", _variant_t(user.password.c_str()));
            m_pRecordset->PutCollect(L"P001", _variant_t(user.p001.c_str()));
            m_pRecordset->PutCollect(L"P002", _variant_t(user.p002.c_str()));
            m_pRecordset->PutCollect(L"P003", _variant_t(user.p003.c_str()));
            m_pRecordset->PutCollect(L"P004", _variant_t(user.p004.c_str()));
            m_pRecordset->PutCollect(L"P005", _variant_t(user.p005.c_str()));
            m_pRecordset->PutCollect(L"P006", _variant_t(user.p006.c_str()));
            m_pRecordset->PutCollect(L"P007", _variant_t(user.p007.c_str()));
            m_pRecordset->PutCollect(L"P008", _variant_t(user.p008.c_str()));
            m_pRecordset->PutCollect(L"P009", _variant_t(user.p009.c_str()));
            m_pRecordset->PutCollect(L"P010", _variant_t(user.p010.c_str()));
            m_pRecordset->Update();
            bUpdated = true;
        }
        m_pRecordset->Close();
        m_pRecordset = nullptr;
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bUpdated;
}

bool CMRKJDatabase::QueryUser(VDBEntryUser &v)
{
    std::wstring sql = L"SELECT * FROM T_USER;";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        while(!m_pRecordset->adoEOF) {
            int id = m_pRecordset->GetCollect(L"ID").intVal;
            DBEntryUser obj{
                StringHelper::ToString(id),
                m_pRecordset->GetCollect(L"USERNAME").bstrVal,
                m_pRecordset->GetCollect(L"PASSWORD").bstrVal,
                m_pRecordset->GetCollect(L"P001").bstrVal,
                m_pRecordset->GetCollect(L"P002").bstrVal,
                m_pRecordset->GetCollect(L"P003").bstrVal,
                m_pRecordset->GetCollect(L"P004").bstrVal,
                m_pRecordset->GetCollect(L"P005").bstrVal,
                m_pRecordset->GetCollect(L"P006").bstrVal,
                m_pRecordset->GetCollect(L"P007").bstrVal,
                m_pRecordset->GetCollect(L"P008").bstrVal,
                m_pRecordset->GetCollect(L"P009").bstrVal,
                m_pRecordset->GetCollect(L"P010").bstrVal
            };
            v.push_back(obj);
            m_pRecordset->MoveNext();
        }
        m_pRecordset->Close ();
        m_pRecordset.Release();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return true;
}


bool CMRKJDatabase::QueryUser(DBEntryUser &u, const wchar_t *szID)
{
    std::wstring sql = L"SELECT * FROM T_USER WHERE ID = ";
    sql += szID;
    sql += L";";
    bool bFind = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            int id = m_pRecordset->GetCollect(L"ID").intVal;
            DBEntryUser obj{
                StringHelper::ToString(id),
                m_pRecordset->GetCollect(L"USERNAME").bstrVal,
                m_pRecordset->GetCollect(L"PASSWORD").bstrVal,
                m_pRecordset->GetCollect(L"P001").bstrVal,
                m_pRecordset->GetCollect(L"P002").bstrVal,
                m_pRecordset->GetCollect(L"P003").bstrVal,
                m_pRecordset->GetCollect(L"P004").bstrVal,
                m_pRecordset->GetCollect(L"P005").bstrVal,
                m_pRecordset->GetCollect(L"P006").bstrVal,
                m_pRecordset->GetCollect(L"P007").bstrVal,
                m_pRecordset->GetCollect(L"P008").bstrVal,
                m_pRecordset->GetCollect(L"P009").bstrVal,
                m_pRecordset->GetCollect(L"P010").bstrVal
            };
            u = obj;
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}

bool CMRKJDatabase::QueryUserByUsername(DBEntryUser &u, const wchar_t *szUsername)
{
    std::wstring sql = L"SELECT * FROM T_USER WHERE USERNAME = '";
    sql += szUsername;
    sql += L"';";
    bool bFind = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            int id = m_pRecordset->GetCollect(L"ID").intVal;
            DBEntryUser obj{
                StringHelper::ToString(id),
                m_pRecordset->GetCollect(L"USERNAME").bstrVal,
                m_pRecordset->GetCollect(L"PASSWORD").bstrVal,
                m_pRecordset->GetCollect(L"P001").bstrVal,
                m_pRecordset->GetCollect(L"P002").bstrVal,
                m_pRecordset->GetCollect(L"P003").bstrVal,
                m_pRecordset->GetCollect(L"P004").bstrVal,
                m_pRecordset->GetCollect(L"P005").bstrVal,
                m_pRecordset->GetCollect(L"P006").bstrVal,
                m_pRecordset->GetCollect(L"P007").bstrVal,
                m_pRecordset->GetCollect(L"P008").bstrVal,
                m_pRecordset->GetCollect(L"P009").bstrVal,
                m_pRecordset->GetCollect(L"P010").bstrVal
            };
            u = obj;
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}

bool CMRKJDatabase::CheckUser(const wchar_t *szUsername, const wchar_t *szPassword)
{
    wchar_t szSql[1024] = {};
    _stprintf_s(szSql
                , _T("SELECT ID FROM T_USER WHERE USERNAME='%s' AND PASSWORD = '%s'")
                , szUsername, szPassword);
    bool bFind = false;
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(szSql, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}


bool CMRKJDatabase::IsExistUsername(const wchar_t *szUsername)
{
    bool bFind = false;
    std::wstring sql = L"SELECT USERNAME FROM T_USER WHERE USERNAME = '";
    sql += szUsername;
    sql += L"';";
    try {
        //����
        _RecordsetPtr m_pRecordset;
        m_pRecordset.CreateInstance(__uuidof(Recordset));
        // ��ѯ,��ý����
        m_pRecordset->Open(sql.c_str(), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
        if(!m_pRecordset->adoEOF) {
            bFind = true;
        }
        m_pRecordset->Close();
    }
    catch(_com_error &e) {
        CTools::MessageBoxFormat(_T("%s"), e.ErrorMessage());
        return false;
    }
    catch(...) {
        TRACE("�����쳣\r\n");
        return false;
    }
    return bFind;
}






