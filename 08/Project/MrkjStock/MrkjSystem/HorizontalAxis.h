#pragma once

/*
    ���ᣬ����
*/

// ǰ������
class CStockChart;

class CHorizontalAxis
{
public:
    CHorizontalAxis(CStockChart* pParent);
    ~CHorizontalAxis();

    // ����λ����Ϣ����С�ȵ�,��Ҫ���ڸ����ڱ��֮ʱ
    void UpdateCtrl();

    // ������ƶ�
    void OnMouseMove(CPoint pt);

    // ���ú�������(�б�)
    void SetDateList(const vector<COleDateTime>& vDate);

    // ����x���꣬�������ǰ������
    COleDateTime XPosToDate(int x) const;

    // �������ڣ������x����
    int DateToXPos(const COleDateTime& date) const;

    // ���Լ�
    void Draw(Gdiplus::Graphics& gh);

    void DrawStartEndDate(int yEnd, Gdiplus::Graphics &gh);

    // ����ÿһ�����ռ�õ�������
    int GetLenOf1Day() const;

    // ���� ���᱾��ռ�õ� RECT
    RECT GetRectAxis() const;

    // ����������ĳ���
    int GetLenOfAxis() const;

private:
    // ����� n����x����
    int GetXPos(int iIndex/*[0,)*/) const;

    // ����� dt �յ� x����
    int GetXPos(const COleDateTime& dt) const;

    // ���ؼ�
    CStockChart* m_pParent;

    //��ʱ�䷶Χ
    COleDateTime m_minDt{ 1999, 1, 1, 0, 0, 0 }, m_maxDt{ 2016, 12, 31, 0, 0, 0 };

    // �ߵ���ɫ
    Gdiplus::Color m_clrLine { Gdiplus::Color(0xFF, 0xFF, 0x00, 0x00) };

    // ��ǩ����
    Gdiplus::Font *m_pFont {new Gdiplus::Font(_T("����"), 8.0f)};
    Gdiplus::SolidBrush *m_pBrush{ new Gdiplus::SolidBrush(m_clrLine)};

    Pen * m_pPenLine{new Pen(m_clrLine)};


    // ��ʼλ��
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };

    // ����λ��
    int m_nXEnd{ 0 };
    int m_nYEnd{ 0 };

    // ���߾���ײ��ľ���
    int m_nOffsetBottom{ 20 };

    // ���߾����Ҳ�ľ���
    int m_nOffsetRight{ 40 };

    // �潲�������ϵ�����
    vector<COleDateTime> m_vDate;

    // �����������,��С����
    void SetTimeMaxMin();
};

