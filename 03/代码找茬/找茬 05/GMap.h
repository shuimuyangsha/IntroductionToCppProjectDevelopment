#pragma once

#include <list>

#define  MAPLENTH 19  //逻辑地图大小
#define P_ROW 10
#define P_ARRAY 9
#define E_ROW 8
#define E_ARRAY 9

using std::list;

//抽象类GMap
class GMap
{

protected:
    static int LD;//障碍物尺寸
    static int PD;//豆子的半径
    void InitOP();//敌我双方出现位置没有豆子出现
    bool mapData[MAPLENTH][MAPLENTH];//障碍物逻辑地图点阵
    bool peaMapData[MAPLENTH][MAPLENTH];//豆子逻辑地图点阵
    COLORREF color;
public:
    void  DrawMap(HDC &hdc);//绘制地图
    void  DrawPeas(HDC &hdc);//绘制豆子
    virtual ~GMap();
    GMap()
    {

    }
    friend class GObject;//允许物体类使用直线的起点和终点的信息做碰撞检测
    friend class PacMan;//允许"大嘴"访问豆子地图
};

//"第一关"
class Stage_1 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_1();
};

//第二关
class Stage_2 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_2();
};

// 第三关
class Stage_3 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_3();
};