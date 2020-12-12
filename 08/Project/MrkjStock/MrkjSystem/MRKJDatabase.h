#pragma once

// ################################################################################
// # ADO 数据库支持
#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
        no_namespace rename("EOF", "adoEOF")


#include "DayDataEntry.h"

// 获取主机名:为了调试时,可以方便连接数据库
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
    // 连接数据库
    bool Connect(const char *host = GetHostName(), const char *database = "MrkjStock", const char *username = "sa", const char *password = "sql2012");
    // 关闭数据库
    void Close();

    // 查询数据库中的表 strCode 长度 [1,7) 是一个不完整的 股票代码,长度越长,匹配的股票表越少
    bool QueryDayDataTables(std::vector<std::wstring> &vTableNames, const std::wstring &strCode);

    // 确保 表存在:如果不存在,要创建数据表,并放到 code_table表中
    // 如果存在,则更新股票名字名字
    bool MakeSureExitsTable(const wchar_t *szCode, const wchar_t *szName);

    // 如果不存在，则创建 日线数据表
    bool MakeDayTable(const std::wstring &strStockCode);

    // 增加股票代码,名称到数据库
    bool AddStock(const std::wstring &strStockCode, const std::wstring &strStockName);
    bool AddStock(const std::vector<std::pair<std::wstring, std::wstring>> &v);

    // 增加 股票信息
    bool AddStockInfo(const wchar_t *code, const wchar_t *name, int nKindId);

    // 更新股票名字
    bool UpdateStockNameByCode(const wchar_t *szCode, const wchar_t *szNewName);

    // 代码,名称, 种类ID, 种类名;
    typedef std::tuple<std::wstring, std::wstring, int, std::wstring> TupleStockInfo;

    // 查询tuple<股票代码,名字,种类ID,种类名>
    bool QueryStockInfo(std::vector<TupleStockInfo> &v);
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<std::wstring> vCodes);
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, std::vector<int> vKinds);

    // 查指指定页数的
    bool QueryStockInfo(std::vector<TupleStockInfo> &v, int iPage, int countOnePage);

    bool QueryStockInfo(std::vector<TupleStockInfo> &v, int kindId);
    // 模糊查询
    bool QueryStockInfoFuzzy(std::vector<TupleStockInfo> &v, const wchar_t *szInput);
    // 查询股票信息, 根据股票代码
    bool QueryStockInfoByCode(TupleStockInfo &info, const wchar_t *szCode);
    bool QueryStockInfoByName(TupleStockInfo &info, const wchar_t *name);
    // 查询股票名字, 根据股票代码
    bool QueryStockNameByCode(const wchar_t *code, std::wstring &name);
    // 增加新股票信息并创建日线数据表, 或者如果己经存在,就更新
    bool UpdStockInfoOrAddTableByStockCode(const TupleStockInfo &info);
    // 更新股票信息
    bool UpdateStockInfoByStockCode(const TupleStockInfo &info);
    // 确认信息是否存在
    bool IsExistsStockInfo(const wchar_t *szStockCode);
    // 删除股票信息
    bool DelStockInfoByCode(const wchar_t *szStockCode);


    /*    日线数据表    */
    // 增加数据
    bool AddDayData(const CDayDataEntry &data);
    bool AddDayData(const VDayDataEntry &v);

    // 判断'今日'数据是否存在
    bool IsExistDayData(const wchar_t *szCode, const wchar_t *szDate);
    // 删除数据: 删除某支股票指定日期的数据
    bool DelDayData(const std::wstring &strStockCode, const std::wstring &date);
    // 修改数据: 根据  data.code and data.date
    bool UpdateDayData(const CDayDataEntry &data);
    // 根据之前的日期进行修改
    bool UpdateDayData(const CDayDataEntry &data, const wchar_t *oldDate);
    // 如果今日数据存在,更新,否则插入新数据
    bool AddOrUpdateDayDataByDate(const CDayDataEntry &data);

    // 查询日数据
    // v : 返回结果
    // szStockCode : 股票代码
    // szDateStart : 查询条件之开始日期, 如果为nullptr则表示不限制开始日期
    // szDateEnd : 查询条件之结束日期, 如果为nullptr则表示不限制结束日期
    bool QueryDayData(VDayDataEntry &v, const wchar_t *szStockCode, const wchar_t *szDateStart = nullptr, const wchar_t *szDataEnd = nullptr);

    /* 板块 */
    // 查询所有
    bool QueryKind(std::vector<std::tuple<int, std::wstring>> &v);
    // 判断板块是否存在
    bool IsExistsKindName(const wchar_t *szName);
    // 查询  种类id
    bool QueryKindIdByName(int &kind_id, const wchar_t *szKindName);
    // 增加板块
    bool AddKind(const wchar_t *szName);
    // 删除板块
    bool DelKind(const wchar_t *szName);
    // 更新板块名
    bool UpdateKind(int id, const wchar_t *szName);

    /*********************************************************************************/
    // 用户及权限管理
    typedef struct {
        std::wstring id;
        std::wstring username;
        std::wstring password;
        std::wstring p001; // 系统权限
        std::wstring p002; // 基础数据维护权限
        std::wstring p003; // 品种维护权限
        std::wstring p004; // 导入导出权限
        std::wstring p005; // 查询权限
        std::wstring p006; // 即时数据权限
        std::wstring p007; // 工具权限
        std::wstring p008; // 金融计算器权限
        std::wstring p009; // 屏幕截图权限
        std::wstring p010; // 系统设置权限

    } DBEntryUser, *PDBEntryUser;
    typedef vector<DBEntryUser> VDBEntryUser;

    // 增加用户
    bool AddUser(const DBEntryUser &user);
    // 删除用户
    bool DelUser(const wchar_t *szID);
    // 更新用户
    bool UpdUser(const DBEntryUser &user);
    // 查询用户
    bool QueryUser(VDBEntryUser &v);
    bool QueryUser(DBEntryUser &u, const wchar_t *szID);
    bool QueryUserByUsername(DBEntryUser &u, const wchar_t *szUsername);
    // 检查用户名密码,是否匹配
    bool CheckUser(const wchar_t *szUsername, const wchar_t *szPassword);
    // 用户名是否存在
    bool IsExistUsername(const wchar_t *szUsername);
private:
    CMRKJDatabase();

    static const std::wstring DAY_NAME_PREFIX;
    //获得表名
    std::wstring GetDayTableName(const std::wstring &code)
    {
        return DAY_NAME_PREFIX + code;
    }

    //指向Connection对象的指针:
    _ConnectionPtr m_pConnection;
};
// 全局唯一 数据库操作对象
#define DB (CMRKJDatabase::Instance())
