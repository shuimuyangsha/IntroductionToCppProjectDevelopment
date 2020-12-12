#include "stdafx.h"
#include "GMap.h"

int GMap::LD = 36;
int GMap::PD = 3;
//敌我双方出现位置没有豆子出现
void GMap::InitOP()
{
    peaMapData[E_ROW][E_ARRAY] = false;
    peaMapData[P_ROW][P_ARRAY] = false;
}
void GMap::DrawMap(HDC &memDC)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    for(int i = 0; i < MAPLENTH; i++) {
        for(int j = 0; j < MAPLENTH; j++) {
            //绘制墙壁
            if(!mapData[i][j]) {
                RECT rect;
                rect.left = j * LD;
                rect.top = i * LD;
                rect.right = (j + 1) * LD;
                rect.bottom = (i + 1) * LD;
                FillRect(memDC, &rect, hBrush);
            }
        }
    }
    DeleteObject(hBrush);
}
void GMap::DrawPeas(HDC &hdc)
{
    for(int i = 0; i < MAPLENTH; i++) {
        for(int j = 0; j < MAPLENTH; j++) {
            if(peaMapData[i][j]) {
                // 画圆
                Ellipse(hdc, (LD / 2 - PD) + j * LD, (LD / 2 - PD) + i * LD, (LD / 2 + PD) + j * LD, (LD / 2 + PD) + i * LD);
            }
        }
    }

    // 如果按下B，直接过关
    if(GetAsyncKeyState('B') & 0x8000) {
        MessageBoxA(NULL, "无意中您发现了秘笈", "", MB_OK);
        for(int i = 0; i < MAPLENTH; i++) {
            for(int j = 0; j < MAPLENTH; j++) {
                peaMapData[i][j] = false;
            }
        }
    }
}
GMap::~GMap()
{

}
//Stage_1成员定义:
#define A true // true:表示豆子
#define B false // false:表示墙壁
bool Stage_1::initData[MAPLENTH][MAPLENTH] = {
    B, B, B, B, B, B, B, B, B, A, B, B, B, B, B, B, B, B, B, //0
    B, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, B, //1
    B, A, A, B, A, A, B, B, B, A, B, B, B, A, A, B, A, A, B, //2
    B, A, B, B, A, A, A, A, A, A, A, A, A, A, A, B, B, A, B, //3
    B, A, B, A, A, A, B, B, B, A, B, B, B, A, A, A, B, A, B, //4
    B, A, B, A, A, A, A, A, A, A, A, A, A, A, A, A, B, A, B, //5
    B, A, A, A, A, A, B, B, A, A, A, B, B, A, A, A, A, A, B, //6
    B, A, B, A, A, A, A, A, A, A, A, A, A, A, A, A, B, A, B, //7
    B, A, B, A, A, A, A, A, B, A, B, A, A, A, A, A, B, A, B, //8
    A, A, A, A, A, A, A, A, B, B, B, A, A, A, A, A, A, A, A, //9
    B, A, B, A, A, A, A, A, A, A, A, A, A, A, A, A, B, A, B, //10
    B, A, B, A, A, B, A, A, A, A, A, A, A, B, A, A, B, A, B, //11
    B, A, B, A, B, B, B, A, A, A, A, A, B, B, B, A, B, A, B, //12
    B, A, A, A, A, B, A, A, A, A, A, A, A, B, A, A, A, A, B, //13
    B, A, B, B, A, A, A, A, A, A, A, A, A, A, A, B, B, A, B, //14
    B, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, B, //15
    B, A, A, A, A, B, B, B, A, B, A, B, B, B, A, A, A, A, B, //16
    B, A, A, A, A, B, A, A, A, A, A, A, A, B, A, A, A, A, B, //17
    B, B, B, B, B, B, B, B, B, A, B, B, B, B, B, B, B, B, B, //18
};
#undef A
#undef B
Stage_1::Stage_1()
{
    color = RGB(140, 240, 240);
    for(int i = 0; i < MAPLENTH; i++) {
        for(int j = 0; j < MAPLENTH; j++) {
            this->mapData[i][j] = this->initData[i][j];
            this->peaMapData[i][j] = this->initData[i][j];
        }
    }
    //敌我双方出现位置没有豆子出现
    this->InitOP();
}
//Stage_2成员定义
#define A true
#define B false
bool Stage_2::initData[MAPLENTH][MAPLENTH] = {
    B, B, B, B, B, B, B, B, B, A, B, B, B, A, B, B, B, B, B, //0
    A, A, A, A, A, A, A, B, A, A, B, A, A, A, B, A, B, A, A, //1
    B, A, A, A, B, A, A, B, A, A, B, A, B, A, B, A, B, A, B, //2
    B, B, B, A, B, A, A, B, B, A, B, A, B, A, B, A, B, B, B, //3
    B, A, A, A, A, A, A, A, A, A, A, A, B, B, B, A, A, A, B, //4
    B, A, A, B, A, A, A, A, A, A, A, A, A, A, A, A, A, A, B, //5
    B, A, A, B, A, A, A, B, B, B, B, B, B, A, A, B, A, A, B, //6
    B, A, A, B, A, B, A, A, A, A, A, A, A, A, A, B, A, A, B, //7
    B, A, A, B, A, B, A, A, B, A, B, A, A, B, A, B, A, A, B, //8
    A, A, A, B, A, B, A, A, B, B, B, A, A, B, A, B, A, A, A, //9
    B, A, A, B, A, B, A, A, A, A, A, A, A, B, A, A, A, A, B, //10
    B, A, A, B, A, A, A, B, B, B, B, B, A, B, A, A, A, A, B, //11
    B, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, B, //12
    B, A, A, A, B, B, B, B, B, B, B, A, A, A, A, A, A, A, B, //13
    B, A, A, A, A, A, A, A, A, A, A, A, A, B, A, A, A, A, B, //14
    B, B, B, B, B, A, A, A, A, B, B, B, A, B, A, A, A, A, B, //15
    B, A, A, A, B, B, B, A, A, A, A, B, A, B, B, B, A, A, B, //16
    A, A, A, A, B, A, A, A, A, A, A, B, A, A, A, B, A, A, A, //17
    B, B, B, B, B, B, B, B, B, A, B, B, B, A, B, B, B, B, B, //18
};
#undef A
#undef B
Stage_2::Stage_2()
{
    color = RGB(240, 140, 140);
    for(int i = 0; i < MAPLENTH; i++) {
        for(int j = 0; j < MAPLENTH; j++) {
            this->mapData[i][j] = this->initData[i][j];
            this->peaMapData[i][j] = this->initData[i][j];
        }
    }
    //敌我双方出现位置没有豆子出现
    this->InitOP();
}
//Stage_3成员定义
#define A true
#define B false
bool Stage_3::initData[MAPLENTH][MAPLENTH] = {
    B, B, B, B, B, B, B, B, B, A, B, B, B, B, B, B, B, B, B, //0
    A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A, //1
    B, A, A, B, A, A, B, B, B, B, B, B, B, A, A, A, B, A, B, //2
    B, A, B, B, A, A, A, A, A, A, A, A, B, A, A, A, B, A, B, //3
    B, A, B, A, A, A, B, B, B, B, B, B, B, A, A, A, B, A, B, //4
    B, A, B, A, B, B, B, A, A, A, A, A, B, B, B, A, B, A, B, //5
    B, A, A, A, B, A, B, A, A, A, A, A, A, A, A, A, B, A, B, //6
    B, A, B, A, B, A, A, A, A, A, A, A, A, B, A, A, B, A, B, //7
    B, A, B, A, B, B, A, A, B, A, B, A, A, B, A, A, B, A, B, //8
    B, A, A, A, A, B, A, A, B, B, B, A, A, B, A, A, B, A, B, //9
    B, A, B, A, A, B, A, A, A, A, A, A, A, B, A, A, A, A, B, //10
    B, A, B, A, A, B, A, A, A, A, A, A, B, B, B, A, B, A, B, //11
    B, A, B, A, A, B, A, B, B, B, B, B, B, A, B, A, B, A, B, //12
    B, A, B, A, A, B, A, A, A, A, A, A, A, A, B, A, B, A, B, //13
    B, A, B, B, A, B, B, B, B, B, B, A, B, A, B, A, B, A, B, //14
    B, A, A, A, A, B, A, A, A, A, A, A, B, A, B, A, B, A, B, //15
    B, B, B, B, B, B, A, A, B, B, B, A, B, A, B, A, B, A, B, //16
    A, A, A, A, A, A, A, A, B, A, A, A, A, A, B, A, A, A, A, //17
    B, B, B, B, B, B, B, B, B, A, B, B, B, B, B, B, B, B, B, //18
};
#undef A
#undef B
Stage_3::Stage_3()
{
    color = RGB(100, 300, 100);
    for(int i = 0; i < MAPLENTH; i++) {
        for(int j = 0; j < MAPLENTH; j++) {
            this->mapData[i][j] = this->initData[i][j];
            this->peaMapData[i][j] = this->initData[i][j];
        }
    }
    //敌我双方出现位置没有豆子出现
    this->InitOP();
}
