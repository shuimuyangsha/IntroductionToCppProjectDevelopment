#pragma once
class CScreenTools
{
private:

    CScreenTools()
    {
    }

    ~CScreenTools()
    {
    }

private:
    //����λͼ�ļ�ÿ��������ռ�ֽ���
    static int GetBits()
    {
        //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
        int iBits;
        HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
        iBits = GetDeviceCaps(hDC, BITSPIXEL) *  GetDeviceCaps(hDC, PLANES);
        DeleteDC(hDC);
        return iBits;
    }
    // ȷ���Ƕ���λ��λͼ
    static int GetBitCount(int iBits)
    {
        int wBitCount;
        if(iBits <= 1) {
            wBitCount = 1;
        }
        else if(iBits <= 4) {
            wBitCount = 4;
        }
        else if(iBits <= 8) {
            wBitCount = 8;
        }
        else if(iBits <= 24) {
            wBitCount = 24;
        }
        else {
            wBitCount = 32;
        }
        return wBitCount;
    }
    static BOOL WriteBmpDataToFile(PCTSTR szFileName, PBITMAPINFOHEADER pBitmapInfoHeader, DWORD dwPaletteSize, DWORD dwBmBitsSize)
    {
        // д����ֽ���
        DWORD dwWrite;
        //λͼ�ļ�ͷ�ṹ
        HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if(hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BITMAPFILEHEADER bitmapFileHeader;
        // ����λͼ�ļ�ͷ
        bitmapFileHeader.bfType = 0x4D42; // "BM"
        bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
        bitmapFileHeader.bfReserved1 = 0;
        bitmapFileHeader.bfReserved2 = 0;
        bitmapFileHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
        // д��λͼ�ļ�ͷ
        WriteFile(hFile, (LPSTR)&bitmapFileHeader, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
        // д��λͼ�ļ���������
        WriteFile(hFile, (LPSTR)pBitmapInfoHeader, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWrite, NULL);
        // �ر��ļ����
        CloseHandle(hFile);
        return TRUE;
    }

public:
    // ���浽�ļ�
    static int SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName)
    {
        /*
            bitmap �ļ��ṹ����:
            BITMAPFILEHEADER
            BITMAPINFOHEADER
            RGBQUAD array
            Color-index array
        */
        //����ֵ
        BOOL bRet = FALSE;
        //λͼ��ÿ��������ռ�ֽ���
        WORD wBitCount;
        //�����ɫ���С�� λͼ�������ֽڴ�С �� λͼ�ļ���С �� д���ļ��ֽ���
        DWORD dwPaletteSize = 0;
        DWORD dwBmBitsSize;
        //λͼ���Խṹ
        BITMAP Bitmap;
        //λͼ��Ϣͷ�ṹ
        BITMAPINFOHEADER bitmapInfoHeader;
        //ָ��λͼ��Ϣͷ�ṹ
        LPBITMAPINFOHEADER pBitmapInfoHeader;
        // ȷ���༸λ(1,4,8,24,32)λͼ
        wBitCount = GetBitCount(GetBits());
        //�����ɫ���С
        if(wBitCount <= 8) {
            dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);
        }
        //����λͼ��Ϣͷ�ṹ
        GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
        bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmapInfoHeader.biWidth = Bitmap.bmWidth;
        bitmapInfoHeader.biHeight = Bitmap.bmHeight;
        bitmapInfoHeader.biPlanes = 1;
        bitmapInfoHeader.biBitCount = wBitCount;
        bitmapInfoHeader.biCompression = BI_RGB;
        bitmapInfoHeader.biSizeImage = 0;
        bitmapInfoHeader.biXPelsPerMeter = 0;
        bitmapInfoHeader.biYPelsPerMeter = 0;
        bitmapInfoHeader.biClrUsed = 0;
        bitmapInfoHeader.biClrImportant = 0;
        // �������ݵĴ�С
        dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 // ÿһ�еĴ�С(������4��������)
                       * Bitmap.bmHeight;
        HANDLE hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
        pBitmapInfoHeader = (LPBITMAPINFOHEADER)GlobalLock(hDib);
        *pBitmapInfoHeader = bitmapInfoHeader;
        // ��ȡ ��ɫ�����µ�����ֵ
        {
            //�豸������
            HDC hDC;
            // ��ȡ�ɵĵ�ɫ��
            HPALETTE hOldPal = NULL;
            // ��ȡ ��ĻDC
            hDC = ::GetDC(NULL);
            // ��ȡĬ�ϵ�ɫ��
            HANDLE hPal = GetStockObject(DEFAULT_PALETTE);
            if(hPal) {
                hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
                RealizePalette(hDC);
            }
            // ��ȡ�õ�ɫ�����µ�����ֵ
            GetDIBits(hDC, hBitmap
                      , 0
                      , (UINT)Bitmap.bmHeight
                      , (LPSTR)pBitmapInfoHeader + sizeof(BITMAPINFOHEADER) + dwPaletteSize // ���
                      , (BITMAPINFO *)pBitmapInfoHeader // ���
                      , DIB_RGB_COLORS);
            //�ָ���ɫ��
            if(hOldPal) {
                SelectPalette(hDC, hOldPal, TRUE);
                RealizePalette(hDC);
            }
            // �ͷ���ĻDC
            ::ReleaseDC(NULL, hDC);
        }
        //����λͼ�ļ�  ,��д��
        {
            if(!WriteBmpDataToFile(lpFileName, pBitmapInfoHeader, dwPaletteSize, dwBmBitsSize)) {
                goto __Cleanup;
            }
        }
        bRet = TRUE;
__Cleanup: //���
        GlobalUnlock(hDib);
        GlobalFree(hDib);
        return bRet;
    }

    // ��ȡ λͼ���
    static HBITMAP CopyScreenToBitmap(LPRECT lpRect) //lpRect ����ѡ������
    {
        HDC hScrDC, hMemDC;
        // ��Ļ���ڴ��豸������
        HBITMAP hBitmap, hOldBitmap;
        // λͼ���
        int nX, nY, nX2, nY2;
        // ѡ����������
        int nWidth, nHeight;
        // λͼ��Ⱥ͸߶�
        int xScrn, yScrn;
        // ��Ļ�ֱ���
        // ȷ��ѡ������Ϊ�վ���
        if(IsRectEmpty(lpRect)) {
            return NULL;
        }
        //Ϊ��Ļ�����豸������
        hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
        //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
        hMemDC = CreateCompatibleDC(hScrDC);
        // ���ѡ����������
        nX = lpRect->left;
        nY = lpRect->top;
        nX2 = lpRect->right;
        nY2 = lpRect->bottom;
        // �����Ļ�ֱ���
        xScrn = GetDeviceCaps(hScrDC, HORZRES);
        yScrn = GetDeviceCaps(hScrDC, VERTRES);
        //ȷ��ѡ�������ǿɼ���
        if(nX < 0) {
            nX = 0;
        }
        if(nY < 0) {
            nY = 0;
        }
        if(nX2 > xScrn) {
            nX2 = xScrn;
        }
        if(nY2 > yScrn) {
            nY2 = yScrn;
        }
        nWidth = nX2 - nX;
        nHeight = nY2 - nY;
        // ����һ������Ļ�豸��������ݵ�λͼ
        hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
        // ����λͼѡ���ڴ��豸��������
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        // ����Ļ�豸�����������ڴ��豸��������
        BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
        // �õ���Ļλͼ�ľ��
        hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
        // ���
        DeleteDC(hScrDC);
        DeleteDC(hMemDC);
        // ����λͼ���
        return hBitmap;
    }

    // ȫ����ͼ
    static HBITMAP PrintScreen()
    {
        CRect rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = GetSystemMetrics(SM_CXSCREEN);
        rect.bottom = GetSystemMetrics(SM_CYSCREEN);
        return CopyScreenToBitmap(rect);
    }

    // ��ָ������
    static HBITMAP PrintWindow(HWND hwnd)
    {
        RECT rrrr;
        LPRECT lpRect = &rrrr;
        ::GetWindowRect(hwnd, &rrrr);
        HDC hScrDC, hMemDC; // ��Ļ���ڴ��豸������
        HBITMAP hBitmap, hOldBitmap; // λͼ���
        //Ϊ��Ļ�����豸������
        hScrDC = ::GetWindowDC(hwnd);
        //Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
        hMemDC = CreateCompatibleDC(hScrDC);
        // λͼ��Ⱥ͸߶�
        int nWidth = lpRect->right - lpRect->left;
        int nHeight = lpRect->bottom - lpRect->top;;
        // ����һ������Ļ�豸��������ݵ�λͼ
        hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
        // ����λͼѡ���ڴ��豸��������
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        // ����Ļ�豸�����������ڴ��豸��������
        BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, 0, 0, SRCCOPY);
        //�õ���Ļλͼ�ľ��
        hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
        //���
        DeleteDC(hScrDC);
        DeleteDC(hMemDC);
        // ����λͼ���
        return hBitmap;
    }

    static void DawMouse(POINT pnt)
    {
        HWND DeskHwnd = ::GetDesktopWindow(); //ȡ��������
        HDC DeskDC = ::GetWindowDC(DeskHwnd); //ȡ�������豸����
        int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);
        HPEN newPen = ::CreatePen(0, 1, RGB(255, 0, 0)); //�����»���,����DeskDC
        HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
        //::Rectangle(DeskDC, pnt.x, pnt.y, pnt.x + 10, pnt.y + 10); //�ڴ�����Χ��ʾ��˸����
        ::MoveToEx(DeskDC, pnt.x - 10, pnt.y, NULL);
        ::LineTo(DeskDC, pnt.x + 10, pnt.y);
        ::MoveToEx(DeskDC, pnt.x, pnt.y + 10, NULL);
        ::LineTo(DeskDC, pnt.x, pnt.y - 10);
        ::SetROP2(DeskDC, oldRop2);
        ::SelectObject(DeskDC, oldPen);
        ::DeleteObject(newPen);
        ::ReleaseDC(DeskHwnd, DeskDC);
        DeskDC = NULL;
    }

};

