#pragma once

#include <list>

#define  MAPLENTH 19  //�߼���ͼ��С
#define P_ROW 10
#define P_ARRAY 9
#define E_ROW 8
#define E_ARRAY 9

using std::list;

//������GMap
class GMap
{

protected:
    static int LD;//�ϰ���ߴ�
    static int PD;//���ӵİ뾶
    void InitOP();//����˫������λ��û�ж��ӳ���
    bool mapData[MAPLENTH][MAPLENTH];//�ϰ����߼���ͼ����
    bool peaMapData[MAPLENTH][MAPLENTH];//�����߼���ͼ����
    COLORREF color;
public:
    void  DrawMap(HDC &hdc);//���Ƶ�ͼ
    void  DrawPeas(HDC &hdc);//���ƶ���
    virtual ~GMap();
    GMap()
    {

    }
    friend class GObject;//����������ʹ��ֱ�ߵ������յ����Ϣ����ײ���
    friend class PacMan;//����"����"���ʶ��ӵ�ͼ
};

//"��һ��"
class Stage_1 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_1();
};

//�ڶ���
class Stage_2 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_2();
};

// ������
class Stage_3 : public GMap
{
private:
    bool static initData[MAPLENTH][MAPLENTH];
public:
    Stage_3();
};