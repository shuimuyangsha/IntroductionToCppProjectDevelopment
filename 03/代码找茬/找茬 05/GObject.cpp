#include "stdafx.h"
#include "GObject.h"

//GOject成员定义:
GMap *GObject::pStage = NULL;

int GObject::GetRow()
{
    return m_nRow;
}

int GObject::GetArray()
{
    return m_nArray;
}

int GObject::PtTransform(int k)
{
    return (k - (pStage->LD) / 2) / pStage->LD;

}

//判断物体是否到达逻辑坐标位置
bool GObject::Achive()
{
    int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD; // 计算x坐标的余数
    int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; // 计算y坐标的余数
    bool l = (n == 0 && k == 0);         // 如果两个余数都为0,说明到达中心位置
    return l;
}

void GObject::AchiveCtrl()
{
    if(Achive()) {
        m_nArray = PtTransform(m_ptCenter.x);  //更新列
        m_nRow = PtTransform(m_ptCenter.y);  //更新行
    }
}

void GObject::DrawBlank(HDC &hdc)
{
    // 申请资源,并交给智能指针处理
    HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
    std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {
        // 离开 DrawBlank函数时,会自动调用释放资源
        DeleteObject(*hbr);
    });
    RECT rect;
    rect.top = m_nY - RD;
    rect.left = m_nX - RD;
    rect.right = m_nX + RD;
    rect.bottom = m_nY + RD;
    // 埴充
    FillRect(hdc, &rect, *phbr);
}

// 设置中心位置
void GObject::SetPosition(int Row, int Array)
{
    m_nRow = Row;
    m_nArray = Array;
    this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
    this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}

// 碰撞检测
bool GObject::Collision()
{
    bool b = false;

    //更新行、列的数据若是大嘴,则会执行PacMan重写的AchiveCtrl函数消除豆子
    AchiveCtrl();
    //判断指令的有效性
    if(m_nArray < 0 || m_nRow < 0 || m_nArray > MAPLENTH - 1
       || m_nRow > MAPLENTH - 1) {
        b = true;
    }
    else if(Achive()) {
        switch(m_cmd) {  //判断行进的方向
            case LEFT:
                //判断下一个格子是否能够通行
                if(m_nArray > 0 &&
                   !pStage->mapData[m_nRow][m_nArray - 1]) {
                    b = true;//指令无效
                }
                break;
            //以下方向的判断原理相同
            case RIGHT:
                if(m_nArray < MAPLENTH - 1 &&
                   !pStage->mapData[m_nRow][m_nArray + 1]) {
                    b = true;
                }
                break;
            case UP:
                if(m_nRow > 0 &&
                   !pStage->mapData[m_nRow - 1][m_nArray]) {
                    b = true;
                }
                break;
            case DOWN:
                if(m_nRow < MAPLENTH - 1 &&
                   !pStage->mapData[m_nRow + 1][m_nArray]) {
                    b = true;
                }
                break;
        }
        if(!b) {
            m_dir = m_cmd; //没撞墙,指令成功
        }
    }
    //依照真实的方向位移
    m_nX = m_ptCenter.x;
    m_nY = m_ptCenter.y;
    int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
    int MIN = pStage->LD / 2;
    switch(m_dir) {  //判断行进的方向
        case LEFT:
            //判断下一个格子是否能够通行
            if(m_nArray > 0 &&
               !pStage->mapData[m_nRow][m_nArray - 1]) {
                b = true;
                break;//"撞墙了"
            }
            m_ptCenter.x -= m_nSpeed;
            if(m_ptCenter.x < MIN) {
                m_ptCenter.x = MAX;
            }

            break;
        //以下方向的判断原理相同
        case RIGHT:
            if(m_nArray < MAPLENTH - 1 &&
               !pStage->mapData[m_nRow][m_nArray + 1]) {
                b = true;
                break;//"撞墙了"
            }
            m_ptCenter.x += m_nSpeed;
            if(m_ptCenter.x > MAX) {
                m_ptCenter.x = MIN;
            }

            break;
        case UP:
            if(m_nRow > 0 &&
               !pStage->mapData[m_nRow - 1][m_nArray]) {
                b = true;
                break;//"撞墙了"
            }
            m_ptCenter.y -= m_nSpeed;
            if(m_ptCenter.y < MIN) {
                m_ptCenter.y = MAX;
            }
            break;
        case DOWN:
            if(m_nRow < MAPLENTH - 1 &&
               !pStage->mapData[m_nRow + 1][m_nArray]) {
                b = true;
                break;//"撞墙了"
            }
            m_ptCenter.y += m_nSpeed;
            if(m_ptCenter.y > MAX) {
                m_ptCenter.y = MIN;
            }
            break;
    }
    return b;
}

//PacMan成员定义:
void PacMan::AchiveCtrl()
{
    GObject::AchiveCtrl();
    if(Achive()) {
        if(m_nRow >= 0 && m_nRow < MAPLENTH &&
           m_nArray >= 0 && m_nArray < MAPLENTH) {  //防止数组越界
            if(pStage->peaMapData[m_nRow][m_nArray]) {
                pStage->peaMapData[m_nRow][m_nArray] = false;
            }
        }
    }
}

void PacMan::action()
{
    Collision();
}
void PacMan::SetTwCommand(TWARDS command)
{
    m_cmd = command;
}

bool PacMan::IsOver()
{
    return m_dir == OVER;
}

bool PacMan::IsWin()
{
    for(int i = 0; i <= MAPLENTH; i++) {
        for(int j = 0; j <= MAPLENTH; j++) {
            if(pStage->peaMapData[i][j] == true) {
                return false; //存在任意一个豆子,没取得胜利
            }
        }
    }
    return true;//没有豆子,胜利
}
POINT PacMan::GetPos()
{
    return m_ptCenter;
}

void PacMan::SetOver()
{
    m_dir = OVER;
}

void PacMan::Draw(HDC &memDC)
{
    if(m_dir == OVER) {

    }
    else if(m_nFrame % 2 == 0) {  //第4祯动画与第2祯动画
        int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        int offsetX = DISTANCE / 2 + D_OFFSET; //弧弦交点
        int offsetY = DISTANCE / 2 + D_OFFSET; //弧弦交点
        switch(m_dir) {
            case UP:
                x1 = m_ptCenter.x - offsetX;
                x2 = m_ptCenter.x + offsetX;
                y2 = y1 = m_ptCenter.y - offsetY;
                break;
            case DOWN:
                x1 = m_ptCenter.x + offsetX;
                x2 = m_ptCenter.x - offsetX;
                y2 = y1 = m_ptCenter.y + offsetY;
                break;
            case LEFT:
                x2 = x1 = m_ptCenter.x - offsetX;
                y1 = m_ptCenter.y + offsetY;
                y2 = m_ptCenter.y - offsetY;
                break;
            case RIGHT:
                x2 = x1 = m_ptCenter.x + offsetX;
                y1 = m_ptCenter.y - offsetY;
                y2 = m_ptCenter.y + offsetY;
                break;

        }
        Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
            m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
            x1, y1,
            x2, y2);
        MoveToEx(memDC, x1, y1, NULL);
        LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
        LineTo(memDC, x2, y2);
    }
    else if(m_nFrame % 3 == 0) {
        Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
                m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);
    }
    else {
        int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        switch(m_dir) {
            case UP:
                x1 = m_ptCenter.x - DISTANCE;
                x2 = m_ptCenter.x + DISTANCE;
                y2 = y1 = m_ptCenter.y;
                break;
            case DOWN:
                x1 = m_ptCenter.x + DISTANCE;
                x2 = m_ptCenter.x - DISTANCE;
                y2 = y1 = m_ptCenter.y;
                break;
            case LEFT:
                x2 = x1 = m_ptCenter.x;
                y1 = m_ptCenter.y + DISTANCE;
                y2 = m_ptCenter.y - DISTANCE;
                break;
            case RIGHT:
                x2 = x1 = m_ptCenter.x;
                y1 = m_ptCenter.y - DISTANCE;
                y2 = m_ptCenter.y + DISTANCE;
                break;

        }

        Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
            m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
            x1, y1,
            x2, y2);
        MoveToEx(memDC, x1, y1, NULL);
        LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
        LineTo(memDC, x2, y2);
    }

    m_nFrame++;//绘制下一祯
}

//Enermy成员定义:
std::shared_ptr<PacMan> Enermy::player = nullptr;

// 抓住，游戏结束
void Enermy::Catch()
{
    int DX = m_ptCenter.x - player->GetPos().x;
    int DY = m_ptCenter.y - player->GetPos().y;
    if((-RD < DX && DX < RD) && (-RD < DY && DY < RD)) {
        player->SetOver();
    }
}
void Enermy::Draw(HDC &hdc)
{
    HPEN pen = ::CreatePen(0, 0, color);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    Arc(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
        m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
        m_ptCenter.x + DISTANCE, m_ptCenter.y,
        m_ptCenter.x - DISTANCE, m_ptCenter.y); //半圆型的头
    int const LEGLENTH = (DISTANCE) / (LEGCOUNTS);
    //根据祯数来绘制身体和“腿部”
    if(m_nFrame % 2 == 0) {
        //矩形的身子
        MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
        LineTo(hdc, m_ptCenter.x - DISTANCE,
               m_ptCenter.y + DISTANCE - LEGLENTH);
        MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
        LineTo(hdc, m_ptCenter.x + DISTANCE,
               m_ptCenter.y + DISTANCE - LEGLENTH);
        for(int i = 0; i < LEGCOUNTS; i++) {  //从左往右绘制“腿部”
            Arc(hdc,
                m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
                m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
                m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
                m_ptCenter.y + DISTANCE,
                m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
                m_ptCenter.y + DISTANCE - LEGLENTH,
                m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
                m_ptCenter.y + DISTANCE - LEGLENTH
               );
        }
    }
    else {
        MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);   //绘制身体
        LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE);
        MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
        LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);
        //从左往右绘制“腿部”

        MoveToEx(hdc, m_ptCenter.x - DISTANCE,
                 m_ptCenter.y + DISTANCE, NULL);
        LineTo(hdc, m_ptCenter.x - DISTANCE + LEGLENTH,
               m_ptCenter.y + DISTANCE - LEGLENTH);

        for(int i = 0; i < LEGCOUNTS - 1; i++) {
            Arc(hdc,
                m_ptCenter.x - DISTANCE + (1 + i * 2)*LEGLENTH,
                m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
                m_ptCenter.x - DISTANCE + (3 + i * 2)*LEGLENTH,
                m_ptCenter.y + DISTANCE,
                m_ptCenter.x - DISTANCE + (1 + i * 2)*LEGLENTH,
                m_ptCenter.y + DISTANCE - LEGLENTH,
                m_ptCenter.x - DISTANCE + (3 + i * 2)*LEGLENTH,
                m_ptCenter.y + DISTANCE - LEGLENTH
               );
        }

        MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, NULL);
        LineTo(hdc, m_ptCenter.x + DISTANCE - LEGLENTH,
               m_ptCenter.y + DISTANCE - LEGLENTH);
    }
    //根据方向绘制眼睛
    int R = DISTANCE / 5; //眼睛的半径
    switch(m_dir) {
        case UP:
            Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y - 2 * R,
                    m_ptCenter.x, m_ptCenter.y);
            Ellipse(hdc, m_ptCenter.x, m_ptCenter.y - 2 * R,
                    m_ptCenter.x + 2 * R, m_ptCenter.y);
            break;
        case DOWN:
            Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y,
                    m_ptCenter.x, m_ptCenter.y + 2 * R);
            Ellipse(hdc, m_ptCenter.x, m_ptCenter.y,
                    m_ptCenter.x + 2 * R, m_ptCenter.y + 2 * R);
            break;
        case LEFT:
            Ellipse(hdc, m_ptCenter.x - 3 * R, m_ptCenter.y - R,
                    m_ptCenter.x - R, m_ptCenter.y + R);
            Ellipse(hdc, m_ptCenter.x - R, m_ptCenter.y - R,
                    m_ptCenter.x + R, m_ptCenter.y + R);
            break;
        case RIGHT:
            Ellipse(hdc, m_ptCenter.x - R, m_ptCenter.y - R,
                    m_ptCenter.x + R, m_ptCenter.y + R);
            Ellipse(hdc, m_ptCenter.x + R, m_ptCenter.y - R,
                    m_ptCenter.x + 3 * R, m_ptCenter.y + R);
            break;
    }

    m_nFrame++; //准备绘制下一祯
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    return;
}
void Enermy::action()
{
    bool b = Collision();
    MakeDecision(b);
    Catch();
}
//RedOne成员

void RedOne::Draw(HDC &hdc)
{
    Enermy::Draw(hdc);
}
void RedOne::MakeDecision(bool b)
{
    //srand(time(0));
    int i = rand();
    if(b) {  //撞到墙壁,改变方向
        //逆时针转向
        if(i % 4 == 0) {
            m_dir == UP ? m_cmd = LEFT : m_cmd = UP;
        }
        else if(i % 3 == 0) {
            m_dir == DOWN ? m_cmd = RIGHT : m_cmd = DOWN;
        }
        else if(i % 2 == 0) {
            m_dir == RIGHT ? m_cmd = UP : m_cmd = RIGHT;
        }
        else {
            m_dir == LEFT ? m_cmd = DOWN : m_cmd = LEFT;
        }
        return;
    }

    if(i % 4 == 0) {
        m_cmd != UP ? m_dir == DOWN : m_cmd == UP;
    }
    else if(i % 3 == 0) {
        m_dir != DOWN ? m_cmd = UP : m_cmd = DOWN;
    }
    else if(i % 2 == 0) {
        m_dir != RIGHT ? m_cmd = LEFT : m_cmd = RIGHT;
    }
    else {
        m_dir != LEFT ? m_cmd = RIGHT : m_cmd = LEFT;
    }

}
//BlueOne成员定义

void BlueOne::Draw(HDC &hdc)
{
    Enermy::Draw(hdc);
}

void BlueOne::MakeDecision(bool b)
{

    const int DR = this->m_nRow - player->GetRow();
    const int DA = this->m_nArray - player->GetArray();
    if(!b && DR == 0) {
        if(DA <= BLUE_ALERT && DA > 0) {  //玩家在左侧边警戒范围s
            m_cmd = LEFT;   //向左移动
            return;
        }
        if(DA < 0 && DA >= -BLUE_ALERT) {  //右侧警戒范围
            m_cmd = RIGHT;//向右移动
            return;
        }
    }
    if(!b && DA == 0) {
        if(DR <= BLUE_ALERT && DR > 0) {  //下方警戒范围
            m_cmd = UP;
            return;
        }
        if(DR < 0 && DR >= -BLUE_ALERT) {  //上方警戒范围
            m_cmd = DOWN;
            return;
        }
    }

    RedOne::MakeDecision(b);  //不在追踪模式时RED行为相同
}

//YellowOne成员定义
void YellowOne::MakeDecision(bool b)
{
    const int DR = this->m_nRow - player->GetRow();
    const int DA = this->m_nArray - player->GetArray();
    if(!b) {
        if(DR * DR > DA * DA) {
            if(DA > 0) {  //玩家在左侧边警戒范围
                m_cmd = LEFT;   //向左移动
                return;
            }
            else if(DA < 0) {  //右侧警戒范围
                m_cmd = RIGHT;//向右移动
                return;
            }
        }
        else {
            if(DR > 0) {  //下方警戒范围
                m_cmd = UP;
                return;
            }
            if(DR < 0) {  //上方警戒范围
                m_cmd = DOWN;
                return;
            }
        }
    }
    RedOne::MakeDecision(b);
}
void YellowOne::Draw(HDC &hdc)
{
    Enermy::Draw(hdc);
}