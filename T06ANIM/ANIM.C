/* FILENAME: ANIM.C
 * PROGRAMMER: AK1
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */
#include <stdio.h>

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define AK1_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Сохраненные мышиные координаты */
static INT
  AK1_MouseOldX, AK1_MouseOldY;


/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimSetPause( BOOL NewPauseFlag )
{
  AK1_Anim.IsPause = NewPauseFlag;
} /* End of 'VG4_AnimSetPause' function */


/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;

  memset(&AK1_Anim, 0, sizeof(ak1ANIM));
  AK1_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  AK1_Anim.hDC = CreateCompatibleDC(hDC);
  AK1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AK1_Anim.hDC, AK1_Anim.hBmFrame);
  AK1_Anim.W = 30;
  AK1_Anim.H = 30;
  AK1_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация ввода */
  GetCursorPos(&pt);
  ScreenToClient(AK1_Anim.hWnd, &pt);
  AK1_MouseOldX = pt.x;
  AK1_MouseOldY = pt.y;
  GetKeyboardState(AK1_Anim.KeysOld);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AK1_Anim.IsPause = FALSE;
  FrameCounter = 0;
} /* End of 'AK1_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < AK1_Anim.NumOfUnits; i++)
  {
    AK1_Anim.Units[i]->Close(AK1_Anim.Units[i], &AK1_Anim);
    free(AK1_Anim.Units[i]);
    AK1_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(AK1_Anim.hBmFrame);
  DeleteDC(AK1_Anim.hDC);
} /* End of 'AK1_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AK1_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(AK1_Anim.hBmFrame);
  AK1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AK1_Anim.hDC, AK1_Anim.hBmFrame);

  /* Сохранение размера */
  AK1_Anim.W = W;
  AK1_Anim.H = H;

  ReleaseDC(AK1_Anim.hWnd, hDC);
} /* End of 'AK1_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  JOYINFOEX ji;
  JOYCAPS jc;

  /* очистка фона */
  SelectObject(AK1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AK1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AK1_Anim.hDC, RGB(0, 0, 0));
  Rectangle(AK1_Anim.hDC, 0, 0, AK1_Anim.W + 1, AK1_Anim.H + 1);
  QueryPerformanceCounter(&li);
  AK1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AK1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!AK1_Anim.IsPause)
    AK1_Anim.DeltaTime = AK1_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    AK1_Anim.DeltaTime = 0;
  }
  AK1_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

   /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(AK1_Anim.Keys);
  for (i = 0; i < 256; i++)
    AK1_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AK1_Anim.KeysClick[i] = AK1_Anim.Keys[i] && !AK1_Anim.KeysOld[i];
  memcpy(AK1_Anim.KeysOld, AK1_Anim.Keys, sizeof(AK1_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  AK1_Anim.MsWheel = AK1_MouseWheel;
  AK1_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(AK1_Anim.hWnd, &pt);
  AK1_Anim.MsX = pt.x;
  AK1_Anim.MsY = pt.y;
  /* относительное перемещение */
  AK1_Anim.MsDeltaX = pt.x - AK1_MouseOldX;
  AK1_Anim.MsDeltaY = pt.y - AK1_MouseOldY;
  AK1_MouseOldX = pt.x;
  AK1_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
  
    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
 
      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(AK1_Anim.JButsOld, AK1_Anim.JButs, sizeof(AK1_Anim.JButs));
        for (i = 0; i < 32; i++)
          AK1_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AK1_Anim.JButsClick[i] = AK1_Anim.JButs[i] && !AK1_Anim.JButsOld[i];

        /* Оси */
        AK1_Anim.JX = AK1_GET_AXIS_VALUE(X);
        AK1_Anim.JY = AK1_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          AK1_Anim.JZ = AK1_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          AK1_Anim.JU = AK1_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          AK1_Anim.JV = AK1_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          AK1_Anim.JR = AK1_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AK1_Anim.JPOV = 0;
          else
            AK1_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }
  
  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    static CHAR Buf[100];

    sprintf(Buf, "FPS: %.5f", AK1_Anim.FPS);
    SetWindowText(AK1_Anim.hWnd, Buf);

    AK1_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AK1_Anim.NumOfUnits; i++)
    AK1_Anim.Units[i]->Response(AK1_Anim.Units[i], &AK1_Anim);

  /* рисование объектов */
  for (i = 0; i < AK1_Anim.NumOfUnits; i++)
  {
    SelectObject(AK1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AK1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AK1_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AK1_Anim.hDC, RGB(0, 0, 0));

    AK1_Anim.Units[i]->Render(AK1_Anim.Units[i], &AK1_Anim);
    QueryPerformanceCounter(&li);
  } 
  FrameCounter++;

} /* End of 'AK1_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(AK1_Anim.hWnd);
  BitBlt(hDC, 0, 0, AK1_Anim.W, AK1_Anim.H, AK1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AK1_Anim.hWnd, hDC);
} /* End of 'AK1_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ak1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimAddUnit( ak1UNIT *Unit )
{
  if (AK1_Anim.NumOfUnits < AK1_MAX_UNITS)
  {
    AK1_Anim.Units[AK1_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AK1_Anim);
  }
} /* End of 'AK1_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AK1_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AK1_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AK1_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AK1_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(AK1_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AK1_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimDoExit( VOID )
{
  PostMessage(AK1_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AK1_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
