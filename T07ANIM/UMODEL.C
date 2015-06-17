/* FILENAME: UCAW.C
 * PROGRAMMER: AK1
 * PURPOSE: Bounce Model unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

static BOOL k = FALSE;

/* Тип представления мяча */
typedef struct tagak1UNIT_MODEL
{
  AK1_UNIT_BASE_FIELDS;

  ak1GEOM Geom; 
  ak1PRIM Pr;
  INT TextId;
  INT Sh;
} ak1UNIT_MODEL;

MATR AK1_ModelMatr = AK1_UNIT_MATR;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID AK1_AnimModelInit( ak1UNIT_MODEL *Uni, ak1ANIM *Ani )
{
  Ani->AngleY = 0;
  Ani->AngleX = 0;
  Ani->PosX = 0;
  Ani->PosY = 0;
  Uni->Sh = 1;
 // Uni->TextId = AK1_TextureLoad("M.BMP");

//  AK1_PrimCreate(&Uni->Pr, AK1_PRIM_TRIMESH, 4, 6, V, I);

//  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  
  AK1_RndPrimMatrConvert = MatrMulMatr(MatrMulMatr(MatrTranslate(0, 0, 0), MatrScale(0.020, 0.020, 0.020)), MatrRotateY(90));
  AK1_GeomLoad(&Uni->Geom, "E:\\SPR02\\Falcon\\FIh48\\UFGC.G3D");
} /* End of 'ak1_AnimModelInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimModelClose( ak1UNIT_MODEL *Uni, ak1ANIM *Ani )
{
  AK1_GeomFree(&Uni->Geom);
  AK1_PrimFree(&Uni->Pr);
} /* End of 'ak1_AnimModelClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimModelResponse( ak1UNIT_MODEL *Uni, ak1ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AK1_AnimDoExit();
  Ani->PosX += 5 * (Ani->JX + 0.00002) * !Ani->IsPause;
  Ani->AngleX = 8 * (Ani->JX + 0.00002) * !Ani->IsPause;
  Ani->PosY += 5 * (Ani->JY + 0.00002) * !Ani->IsPause;
  Ani->AngleY = 8 * (Ani->JY + 0.00002) * !Ani->IsPause;
 /* if (Ani->JZ < -0.00002)
    Ani->AngleX += 20 * Ani->JZ * !Ani->IsPause;
  if (Ani->JZ > 0.0)
    Ani->AngleX += 20 * Ani->JZ * !Ani->IsPause;
  if (Ani->JR < -0.00002)
    Ani->AngleY += 20 * Ani->JR * !Ani->IsPause;
  if (Ani->JR > 0.00002)
    Ani->AngleY += 20 * Ani->JR * !Ani->IsPause; */
  if (Ani->JButsClick[9])  
    AK1_AnimFlipFullScreen();
  if (Ani->JButsClick[4])
    AK1_AnimSetPause(!Ani->IsPause);
  if (Ani->JButs[5])
    AK1_AnimDoExit();
} /* End of 'ak1_AnimModelResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimModelRender( ak1UNIT_MODEL *Uni, ak1ANIM *Ani )
{
 // INT i, j;
  AK1_RndMatrView = MatrView(VecSet(0, 1, -15),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  AK1_ModelMatr =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrTranslate(4.5, -3, 0),
      MatrRotateY(Ani->AngleX)),
      MatrRotateX(Ani->AngleY)),
      MatrScale(1, 1, 1));
  AK1_GeomDraw(&Uni->Geom, Uni->Sh);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AK1_PrimDraw(&Uni->Pr, Uni->Sh);
} /* End of 'ak1_AnimModelRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_UnitModelCreate( VOID )
{
  ak1UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AK1_AnimModelInit;
  Uni->Close = (VOID *)AK1_AnimModelClose;
  Uni->Response = (VOID *)AK1_AnimModelResponse;
  Uni->Render = (VOID *)AK1_AnimModelRender;
  return (ak1UNIT *)Uni;
} /* End of 'ak1_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
