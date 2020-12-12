#include "stdafx.h"
#include <time.h>

#include "GMap.h"
#define PLAYERSPEED 6            //����ٶ�
#define ENERMYSPEED 4            //�����ٶ�
#define LEGCOUNTS 5              //�����ȵ�����
#define DISTANCE 10              //ͼ�ͷ�Χ
#define BLUE_ALERT 8             //��ɫ���䷶Χ
#define D_OFFSET   2             //��ͼ���
#define RD (DISTANCE + D_OFFSET) //��ͼ��Χ 12 

//����ö��
enum TWARDS {
    UP,    // ��
    DOWN,  // ��
    LEFT,  // ��
    RIGHT, // ��
    OVER,  // ��Ϸ����
};

// ������ : ����͵��˵ĸ���
class GObject
{
public:
    GObject(int Row, int Array)
    {
        // ֡��
        m_nFrame = 1;
        // ��ǰ�ؿ�
        pStage = NULL;
        // ��
        this->m_nRow = Row;
        // ����
        this->m_nArray = Array;
        // ����λ��
        this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
        this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

        this->m_nX = m_ptCenter.x;
        this->m_nY = m_ptCenter.y;
    }

    // ����λ��
    void SetPosition(int Row, int Array);
    // ���հ�
    void DrawBlank(HDC &hdc);
    void virtual Draw(HDC &hdc) = 0; //���ƶ���
    void virtual action() = 0;//���ݱ���ı���

    int GetRow();
    int GetArray();

    static GMap *pStage; //ָ���ͼ���ָ��,����Ϊ��̬,ʹ������������ܹ�ʹ����ͬ�ĵ�ͼ

protected:
    int m_nX;
    int m_nY;
    //ָ���
    TWARDS m_cmd;
    //��������
    POINT m_ptCenter;
    //�߼�������
    int m_nRow;
    //�߼�������
    int m_nArray;
    //�ٶ�
    int m_nSpeed;
    //����
    TWARDS m_dir;
    //����
    int m_nFrame;
    //�ж������Ƿ񵽴��߼�����λ��
    bool Achive();
    //�߼���ײ���,������ڷŵ������λ��
    bool Collision();
    //��ʵ������ת��Ϊ�߼�����
    int PtTransform(int k);
    //�����߼�����������
    virtual void AchiveCtrl();
};

//��Ҷ���
class PacMan : public GObject
{
protected:
    virtual void AchiveCtrl();//��д�麯��

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

//�о�����
class Enermy : public GObject
{
protected:
    void Catch();//�Ƿ�ץס���
                 // ȷ������
    void virtual MakeDecision(bool b) = 0;//AIʵ��
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

// ���� ����
class RedOne : public Enermy //�漴�ƶ�S
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

class BlueOne : public RedOne //������
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

class YellowOne : public RedOne //������
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