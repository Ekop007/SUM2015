/* FILENAME: UBALL.C
 * PROGRAMMER: AK6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagak1UNIT_BALL
{
  AK1_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} ak1UNIT_BALL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitInit( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AK1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitClose( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
} /* End of 'AK1_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitResponse( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
} /* End of 'AK1_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1NIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitRender( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time * 300 / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'AK1_AnimUnitRender' function */

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_UnitBallCreate( VOID )
{
  ak1UNIT_BALL *Uni;
  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_BALL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AK1_AnimUnitInit;
  Uni->Close = (VOID *)AK1_AnimUnitClose;
  Uni->Response = (VOID *)AK1_AnimUnitResponse;
  Uni->Render = (VOID *)AK1_AnimUnitRender;
  return (ak1UNIT *)Uni;
} /* End of 'AK1_UnitBallCreate' function */


/* END OF 'UBALL.C' FILE */
