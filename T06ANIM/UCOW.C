/* FILENAME: UCAW.C
 * PROGRAMMER: AK1
 * PURPOSE: Bounce cow unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

static BOOL k = FALSE;

/* Тип представления мяча */
typedef struct tagak1UNIT_COW
{
  AK1_UNIT_BASE_FIELDS;

  ak1GOBJ Model;     /* Млдель корова */
} ak1UNIT_COW;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID AK1_AnimCowInit( ak1UNIT_COW *Uni, ak1ANIM *Ani )
{
  AK1_RndGObjLoad(&Uni->Model, "cow.object");
  Ani->PosX = 0;
  Ani->PosY = 0;
} /* End of 'ak1_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimCowClose( ak1UNIT_COW *Uni, ak1ANIM *Ani )
{
  AK1_RndGObjFree(&Uni->Model);
} /* End of 'ak1_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimCowResponse( ak1UNIT_COW *Uni, ak1ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AK1_AnimDoExit();
  if (Ani->JX > 0.0)
    Ani->PosX += 10 * (Ani->JX + 0.00002) * !Ani->IsPause;
  if (Ani->JX < -0.00003)
    Ani->PosX += 10 * (Ani->JX + 0.00002) * !Ani->IsPause;
  if (Ani->JY > 0.0)
    Ani->PosY += 10 * (Ani->JY + 0.00002) * !Ani->IsPause;
  if (Ani->JY < -0.00002)
    Ani->PosY += 10 * (Ani->JY + 0.00002) * !Ani->IsPause;
  if (Ani->JZ < -0.00002)
    Ani->AngleX += 20 * Ani->JZ * !Ani->IsPause;
  if (Ani->JZ > 0.0)
    Ani->AngleX += 20 * Ani->JZ * !Ani->IsPause;
  if (Ani->JR < -0.00002)
    Ani->AngleY += 20 * Ani->JR * !Ani->IsPause;
  if (Ani->JR > 0.00002)
    Ani->AngleY += 20 * Ani->JR * !Ani->IsPause;
  if (Ani->JButsClick[0])
    AK1_AnimFlipFullScreen();
  if (Ani->JButsClick[4])
    AK1_AnimSetPause(!Ani->IsPause);
  if (Ani->JButs[5])
    AK1_AnimDoExit();
} /* End of 'ak1_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimCowRender( ak1UNIT_COW *Uni, ak1ANIM *Ani )
{
  int i, l = -1;
  DBL X1 = Ani->PosX;
  AK1_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrRotateX(Ani->AngleX), MatrRotateY(Ani->AngleY)), MatrScale(10, 10, 10));
  AK1_RndMatrView = MatrView(VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
//  AK1_RndGObjDraw(&Uni->Model);
  for (i = 0; i < 3; i++)
  {
    Ani->PosX = X1 + (i + l) * 600;
    AK1_RndGObjDraw(&Uni->Model);
  }
  Ani->PosX = X1;
} /* End of 'ak1_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_UnitCowCreate( VOID )
{
  ak1UNIT_COW *Uni;

  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AK1_AnimCowInit;
  Uni->Close = (VOID *)AK1_AnimCowClose;
  Uni->Response = (VOID *)AK1_AnimCowResponse;
  Uni->Render = (VOID *)AK1_AnimCowRender;
  return (ak1UNIT *)Uni;
} /* End of 'ak1_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
