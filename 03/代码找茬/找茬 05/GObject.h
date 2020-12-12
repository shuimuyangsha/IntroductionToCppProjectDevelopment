#include "stdafx.h"
#include <time.h>

#include "GMap.h"
#define PLAYERSPEED 6            //玩家速度
#define ENERMYSPEED 4            //敌人速度
#define LEGCOUNTS 5              //敌人腿的数量
#define DISTANCE 10              //图型范围
#define BLUE_ALERT 8             //蓝色警戒范围
#define D_OFFSET   2             //绘图误差
#define RD (DISTANCE + D_OFFSET) //绘图范围 12 

//方向枚举
enum TWARDS {
    UP,    // 上
    DOWN,  // 下
    LEFT,  // 左
    RIGHT, // 右
    OVER,  // 游戏结束
};

// 物体类 : 大嘴和敌人的父类
class GObject
{
public:
    GObject(int Row, int Array)
    {
        // 帧数
        m_nFrame = 1;
        // 当前关卡
        pStage = NULL;
        // 行
        this->m_nRow = Row;
        // 数组
        this->m_nArray = Array;
        // 中心位置
        this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
        this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

        this->m_nX = m_ptCenter.x;
        this->m_nY = m_ptCenter.y;
    }

    // 设置位置
    void SetPosition(int Row, int Array);
    // 画空白
    void DrawBlank(HDC &hdc);
    void virtual Draw(HDC &hdc) = 0; //绘制对象
    void virtual action() = 0;//数据变更的表现

    int GetRow();
    int GetArray();

    static GMap *pStage; //指向地图类的指针,设置为静态,使所有自类对象都能够使用相同的地图

protected:
    int m_nX;
    int m_nY;
    //指令缓存
    TWARDS m_cmd;
    //中心坐标
    POINT m_ptCenter;
    //逻辑横坐标
    int m_nRow;
    //逻辑纵坐标
    int m_nArray;
    //速度
    int m_nSpeed;
    //朝向
    TWARDS m_dir;
    //祯数
    int m_nFrame;
    //判断物体是否到达逻辑坐标位置
    bool Achive();
    //逻辑碰撞检测,将物体摆放到合理的位置
    bool Collision();
    //将实际坐标转换为逻辑坐标
    int PtTransform(int k);
    //到达逻辑点后更新数据
    virtual void AchiveCtrl();
};

//玩家对象
class PacMan : public GObject
{
protected:
    virtual void AchiveCtrl();//重写虚函数

public:
    POINT GetPos();
    bool IsOver();
    bool IsWin();
    void Draw(HDC &hdc);
    void SetTwCommand(TWARDS command);
    PacMan(int x, int y) : GObject(x, y)
    {
        this->m_nSpeed = PLAYERSPEED;
        m_cmd = m_dir = LEFT;
    }
    void action();
    void SetOver();
};

//敌军对象
class Enermy : public GObject
{
protected:
    void Catch();//是否抓住玩家
                 // 确定方向
    void virtual MakeDecision(bool b) = 0;//AI实现
    COLORREF color;
public:
    static std::shared_ptr<PacMan> player;
    void virtual  Draw(HDC &hdc);
    Enermy(int x, int y) : GObject(x, y)
    {
        this->m_nSpeed = ENERMYSPEED;
        m_dir = LEFT;
        m_cmd = UP;
    }
    void virtual action();
};

// 三种 敌人
class RedOne : public Enermy //随即移动S
{
protected:
    void virtual MakeDecision(bool b);
public:
    void Draw(HDC &hdc);
    RedOne(int x, int y) : Enermy(x, y)
    {
        color = RGB(255, 0, 0);
    }
};

class BlueOne : public RedOne //守卫者
{
protected:
    void virtual MakeDecision(bool b);
public:
    void Draw(HDC &hdc);
    BlueOne(int x, int y) : RedOne(x, y)
    {
        color = RGB(0, 0, 255);
    }

};

class YellowOne : public RedOne //扰乱者
{
protected:
    void virtual MakeDecision(bool b);
public:
    void Draw(HDC &hdc);
    YellowOne(int x, int y) : RedOne(x, y)
    {
        color = RGB(200, 200, 100);
    }
};