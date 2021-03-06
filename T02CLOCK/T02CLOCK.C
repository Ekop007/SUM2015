/* T01FWIN.C
 * ������ �������� ������ ��������� ��� WinAPI.
 * �������� � ��������� ����������� ����.
 */
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"
 /* �� */
DOUBLE Pi = 3.14159265358979323846; 
/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
VOID DrawArrow( INT h, INT w, INT bml, HDC hMemDC, DOUBLE angle ) /* ��������� ������� */
{
  INT x, y;
  x = bml * sin(angle * Pi / 180);
  y = bml * cos(angle * Pi / 180);
  SelectObject(hMemDC, GetStockObject(DC_PEN));
  SetDCPenColor(hMemDC, RGB(255, 0, 0));
  MoveToEx(hMemDC, w, h, NULL);
  LineTo(hMemDC, w + x, h - y);
}

VOID DrawHand( HDC hDC, INT Xc, INT Yc, INT L, INT W, DOUBLE Angle)
{
  INT i;
  FLOAT si = sin(Angle), co = cos(Angle);
  POINT pnts[4] = 
  {
    {-W, 0}, {0, L}, {W, 0}, {0, -W}
  }, pntdraw[sizeof(pnts) / sizeof(pnts[0])];
  for(i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pntdraw[i].x = Xc + pnts[i].x * co - pnts[i].y * si; 
    pntdraw[i].y = Yc + pnts[i].y * co + pnts[i].x * si;
  }
  Polygon(hDC, pntdraw, sizeof(pnts) / sizeof(pnts[0]));
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
   HDC hDC;
  INT x, y;
  CREATESTRUCT *cs;
//  POINT pt;
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;
  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);
    hBmLogo = LoadImage(NULL, "Clock1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);

    /* ������� �������� � ������ */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* ������� �������� */
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);
    return 0;

  case WM_TIMER:
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    StretchBlt(hMemDC, w / 2 - bm.bmWidth / 2, h / 2 - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    GetLocalTime(&st);

    SetDCBrushColor(hMemDC, RGB(100, 150, 200));
    DrawHand(hMemDC, w / 2, h / 2, bm.bmWidth / 3.5, bm.bmWidth / 10, (180 + (st.wHour % 12) * 30) * Pi / 180);
    SetDCBrushColor(hMemDC, RGB(200, 55, 130));
    DrawHand(hMemDC, w / 2, h / 2, bm.bmWidth / 2.5, bm.bmWidth / 15, (180 + st.wMinute * 6) * Pi / 180);
    SetDCBrushColor(hMemDC, RGB(200, 100, 10));
    DrawHand(hMemDC, w / 2, h / 2, bm.bmWidth / 2, bm.bmWidth / 20, (180 + st.wSecond * 6) * Pi / 180);

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hMemDC, hFnt);
    GetLocalTime(&st);
    SetTextColor(hMemDC, RGB(0, 255, 0));
    SetBkColor(hMemDC, RGB(255, 255, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    
    TextOut(hMemDC, 5 * w / 12, 11 * h / 12, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear));

    DeleteObject(hFnt);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;
  
 /* case WM_KEYDOWN
    if ()
  */

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    x = w / 8 * sin(clock());
    y = h / 8 * cos(clock());
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(255, 0, 0));
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2, y);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

 /* case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    if (wParam & MK_LBUTTON)
    {
      Ellipse(hMemDC, x - 5, y - 5, x + 5, y + 5);
    }
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(255, 0, 0));
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, x, y);
    return 0;  */

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02DBLB.C' FILE */
