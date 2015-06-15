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

ak1ANIM AK1_Anim;
/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimSetPause( BOOL NewPauseFlag )
{
  AK1_Anim.IsPause = NewPauseFlag;
} /* End of 'AK1_AnimSetPause' function */


/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL AK1_AnimInit( HWND hWnd )
{
  LARGE_INTEGER li;
  POINT pt;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&AK1_Anim, 0, sizeof(ak1ANIM));
  AK1_Anim.hWnd = hWnd;
  AK1_Anim.hDC = GetDC(hWnd);
  AK1_Anim.W = 30;
  AK1_Anim.H = 30;
  AK1_Anim.AngleX = 0;
  AK1_Anim.AngleY = 0;
  AK1_Anim.NumOfUnits = 0;
  
  /*** Инициализация OpenGL ***/

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AK1_Anim.hDC, &pfd);
  DescribePixelFormat(AK1_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(AK1_Anim.hDC, i, &pfd);

  AK1_Anim.hGLRC = wglCreateContext(AK1_Anim.hDC);

  wglMakeCurrent(AK1_Anim.hDC, AK1_Anim.hGLRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AK1_Anim.hGLRC);
    ReleaseDC(AK1_Anim.hWnd, AK1_Anim.hDC);
    memset(&AK1_Anim, 0, sizeof(ak1ANIM));
    return FALSE;
  }

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
  
  AK1_RndProg = AK1_ShaderLoad("TEST"); /* загрузка шейдера */
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);
  return TRUE;
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

  AK1_ShaderFree(AK1_RndProg);
  AK1_RndProg = 0;

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AK1_Anim.hGLRC);

  ReleaseDC(AK1_Anim.hWnd, AK1_Anim.hDC);
} /* End of 'AK1_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimResize( INT W, INT H )
{
  /* Сохранение размера */
  AK1_Anim.W = W;
  AK1_Anim.H = H;

  glViewport(0, 0, W, H);

  if (W > H)
    AK1_RndWp = (DBL)W / H * 3, AK1_RndHp = 3;
  else
    AK1_RndHp = (DBL)H / W * 3, AK1_RndWp = 3;
  AK1_RndMatrProj = MatrFrustum(-AK1_RndWp / 2, AK1_RndWp / 2,
                                -AK1_RndHp / 2, AK1_RndHp / 2,
                                AK1_RndProjDist, 800);
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

  /*  */
  
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

  /* вычисление FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AK1_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  TimeOld = li.QuadPart;

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

  for (i = 0; i < AK1_Anim.NumOfUnits; i++)
    AK1_Anim.Units[i]->Response(AK1_Anim.Units[i], &AK1_Anim);

  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (i = 0; i < AK1_Anim.NumOfUnits; i++)
  {
    static DBL time = 1;

    time += AK1_Anim.GlobalDeltaTime;
    if (time > 1)
    {
      time = 0;
      AK1_ShaderFree(AK1_RndProg);
      AK1_RndProg = AK1_ShaderLoad("TEST");
    }
    AK1_RndMatrWorld = MatrIdentity();

    AK1_Anim.Units[i]->Render(AK1_Anim.Units[i], &AK1_Anim);
  }

  glFinish();
  FrameCounter++;

} /* End of 'AK1_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimCopyFrame( VOID )
{
  /* вывод кадра */
  SwapBuffers(AK1_Anim.hDC);
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
