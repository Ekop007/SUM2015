/* FILENAME: MAIN.C
 * PROGRAMMER: AK1
 * PURPOSE: Animation startup module
 * LAST UPDATE: 06.06.2015
 */

#include "anim.h"
#include "units.h"

#define WND_CLASS_NAME "My Window Class Name"

/* Глобальная переменная - счетчик прокрутки колеса мыши */
INT AK1_MouseWheel;


/* Ссылки вперед */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 * АРГУМЕНТЫ:
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 *   - флаг показа окна (см. SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_***):
 *       INT ShowCmd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  INT i;
  WNDCLASSEX wc;
  HWND hWnd;
  MSG msg;

  /* Регистрация - создание собственного класса окна */
  wc.cbSize = sizeof(WNDCLASSEX);      /* Размер структуры для совместимости */
  wc.style = CS_VREDRAW | CS_HREDRAW;  /* Стиль окна: полностью перерисовывать
                                        * при изменении вертикального или
                                        * горизонтального размеров (еще CS_DBLCLKS) */
  wc.cbClsExtra = 0;                   /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0;                   /* Дополнительное количество байт для окна */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;      /* Фоновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);     /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);   /* Загрузка пиктограммы (системной) */
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* Загрузка малой пиктограммы (системной) */
  wc.lpszMenuName = NULL;                       /* Имя ресурса меню */
  wc.hInstance = hInstance;                     /* Дескриптор приложения, регистрирующего класс */
  wc.lpfnWndProc = MainWindowFunc;              /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;            /* Имя класса */

  /* Регистрируем класс */
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,          /* Стиль окна - перекрывающееся */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
    NULL,                         /* Дескриптор родительского окна */
    NULL,                         /* Дескриптор загруженного меню */
    hInstance,                    /* Дескриптор приложения */
    NULL);                        /* Указатель на дополнительные параметры */

  /* Отобразить с заданными параметрами */
  ShowWindow(hWnd, ShowCmd);
  /* Отрисовать немедленно */
  UpdateWindow(hWnd);

  /*** Добавление объектов ***/
  AK1_AnimAddUnit(AK1_UnitControlCreate());

 /* for (i = 0; i < 100; i++)
    AK1_AnimAddUnit(AK1_UnitBallCreate());*/

/*  AK1_AnimAddUnit(AK1_UnitCowCreate()); */

  /* Запуск цикла обработки сообщений */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* Дополнительная обработка сообщений от клавиатуры (->WM_CHAR) */
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения:
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    AK1_AnimInit(hWnd);
    AK1_AnimAddUnit(AK1_UnitCowCreate());
    return 0;
  case WM_SIZE:
    AK1_AnimResize(LOWORD(lParam), HIWORD(lParam));
    AK1_AnimRender();
    return 0;
  case WM_TIMER:
    AK1_AnimRender();
    AK1_AnimCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_MOUSEWHEEL:
    AK1_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AK1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    AK1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MainWindowFunc' function */

/* END OF 'STARTUP.C' FILE */


