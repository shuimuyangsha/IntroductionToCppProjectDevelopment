#pragma once


// ǰ������
class CStockChart;

class CLeftTopLabel
{
public:
    CLeftTopLabel(CStockChart* pParent);
    ~CLeftTopLabel();

    // ����λ����Ϣ����С�ȵ�
    void UpdateCtrl();

    // ������ʾ���ı�
    void SetText(const wchar_t *szLbl);

    // ���Լ�
    void Draw(Gdiplus::Graphics& gh);

private:
    // ���ؼ�
    CStockChart *m_pParent;
    // ��ʾ���ı�
    std::wstring m_strLabel{ L"δ����" };
    // �ı� ����
    Gdiplus::Font* m_pFont{new Gdiplus::Font(L"����", 10.0f, FontStyleRegular, UnitPoint) };
    // �ı�����
    Gdiplus::Color* m_pColor{new Gdiplus::Color(0xFF, 0x00, 0xFF, 0xFF) };
    // ��ʼλ��
    int m_nXBegin{ 0 };
    int m_nYBegin{ 0 };
};

