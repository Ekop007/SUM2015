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
  VEC CPos, Pos, At;           /* Позиция юнита, камеры */
  FLT Head, Omega, V;
  UINT ProgId;
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
  Uni->ProgId = 1;
 // Uni->TextId = AK1_TextureLoad("M.BMP");

//  AK1_PrimCreate(&Uni->Pr, AK1_PRIM_TRIMESH, 4, 6, V, I);

//  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  
  AK1_RndPrimMatrConvert = MatrMulMatr(MatrMulMatr(MatrTranslate(0, 0, 0), MatrScale(0.020, 0.020, 0.020)), MatrRotateY(90));
  AK1_GeomLoad(&Uni->Geom, "E:SPR02\\Falcon\\FIh48\\UFGC.G3D");
  Uni->ProgId = 0;
  Uni->Pos = VecSet(0, 0, 0);
  Uni->CPos = VecSet(0, 8, 40);
  Uni->V = 0;
  Uni->Head = 0;
  Uni->Omega = 0;
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
  VEC Dir, Move;
  if (Ani->Keys[VK_ESCAPE])
    AK1_AnimDoExit();
  if (Ani->JPOV == 1)
    AK1_AnimFlipFullScreen();
  if (Ani->JButsClick[4])
    AK1_AnimSetPause(!Ani->IsPause);
  if (Ani->JButs[5])
    AK1_AnimDoExit();
  Uni->Head += 300 * Ani->JR * Ani->DeltaTime;
  Uni->Omega += 300 * Ani->JZ * Ani->DeltaTime;
  Dir = VecMulMatr(VecSet(0, 0, 1), MatrMulMatr(MatrRotateY(Uni->Head), MatrRotateX(Uni->Omega)));
  Uni->V += -3 * 30 * Ani->JY * Ani->DeltaTime;
  Uni->V *= max(1 - Ani->GlobalDeltaTime, 0.01);
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Dir, Uni->V * Ani->DeltaTime));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(VecMulMatr(Dir, MatrRotateY(-90)), 30 * Ani->JX * Ani->DeltaTime));
//  Uni->Pos.Y += 300 * (Ani->JButs[1] - Ani->JButs[2]) * Ani->DeltaTime;

  Uni->At = VecSubVec(Uni->Pos, VecMulNum(Dir, 12));
  Uni->At.Y += 4;
  Uni->At.Z -= 10;

  Move = VecSubVec(Uni->At, Uni->CPos);
  Uni->CPos = VecAddVec(Uni->CPos, VecMulNum(Move, Ani->GlobalDeltaTime));
  
  AK1_RndMatrView = MatrView(Uni->CPos,
                             Uni->Pos,
                             VecSet(0, 1, 0));
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

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  AK1_RndMatrWorld = MatrIdentity();
  AK1_ModelMatr =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrTranslate(4.5, -3, 0),
      MatrRotateY(Uni->Head)),
      MatrRotateX(Uni->Omega)),
      MatrTranslate(Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z)),
      MatrScale(1, 1, 1)); 
  AK1_GeomDraw(&Uni->Geom, Uni->ProgId);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AK1_PrimDraw(&Uni->Pr, Uni->ProgId);
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
