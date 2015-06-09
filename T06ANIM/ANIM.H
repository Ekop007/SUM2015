/* FILENAME: ANIM.H
 * PROGRAMMER: AK1
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Максимальное количество элементов анимации */
#define AK1_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagak1UNIT ak1UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HBITMAP hBmFrame;   /* Буфер кадра */
  HDC hDC;            /* Контекст вывода */

  /* Массив элементов анимации и их количество */
  ak1UNIT *Units[AK1_MAX_UNITS]; 
  INT NumOfUnits;
  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */
  /* Подсистема ввода */
  BYTE
    Keys[256],       /* Сотояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Сотояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel,
    PosX, PosY;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Сотояние кнопок джойстика */
    JButsOld[32],    /* Сотояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* Оси джойстика (-1.0 .. 1.0*/

} ak1ANIM;

/* Глобальная переменная - счетчик прокрутки колеса мыши */
extern INT AK1_MouseWheel;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( ak1UNIT *Unit, ak1ANIM *Ani );
 */
#define AK1_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ak1UNIT *Unit, ak1ANIM *Ani );       \
  VOID (*Close)( ak1UNIT *Unit, ak1ANIM *Ani );      \
  VOID (*Response)( ak1UNIT *Unit, ak1ANIM *Ani );   \
  VOID (*Render)( ak1UNIT *Unit, ak1ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagak1UNIT
{
  AK1_UNIT_BASE_FIELDS;
};


extern ak1ANIM AK1_Anim;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ak1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimAddUnit( ak1UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimDoExit( VOID );

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
