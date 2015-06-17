/* FILENAME: UPLANETS.C
 * PROGRAMMER: AK1
 * PURPOSE: Bounce Planet unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"
#include "render.h"

static BOOL k = FALSE;

/* Тип представления мяча */
typedef struct tagak1UNIT_Planet
{
  AK1_UNIT_BASE_FIELDS;

  ak1GEOM Geom; 
  ak1PRIM Pr;
  INT TextId;
  FLT Angle;
  INT Sh;
} ak1UNIT_Planet;

MATR AK1_PlanetMatr = AK1_UNIT_MATR;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

static VOID AK1_AnimPlanetInit( ak1UNIT_Planet *Uni, ak1ANIM *Ani )
{
  ak1MATERIAL Mtl = AK1_MtlLib[0];
  Uni->Angle = Ani->GlobalTime * 0.5;
 
//  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  
 // AK1_RndPrimMatrConvert = MatrMulMatr(MatrMulMatr(MatrTranslate(0, 0, 0), MatrScale(20, 0.020, 0.020)), MatrRotateY(90));
  AK1_PrimCreatePlanet(&Uni->Pr, VecSet(0, 0, 0), 50, 5, "mars1.bmp");
  Mtl.TexId = AK1_TextureLoad("mars.bmp");
  Uni->Sh = 2;
  Uni->Pr.MtlNo = Uni->TextId = AK1_MtlAdd(&Mtl);
} /* End of 'ak1_AnimPlanetInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimPlanetClose( ak1UNIT_Planet *Uni, ak1ANIM *Ani )
{
  AK1_GeomFree(&Uni->Geom);
  AK1_PrimFree(&Uni->Pr);
} /* End of 'ak1_AnimPlanetClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimPlanetResponse( ak1UNIT_Planet *Uni, ak1ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AK1_AnimDoExit();
} /* End of 'ak1_AnimPlanetResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimPlanetRender( ak1UNIT_Planet *Uni, ak1ANIM *Ani )
{
 // INT i, j;
  AK1_RndMatrView = MatrView(VecSet(0, 2, 100),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  AK1_PlanetMatr =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrRotateY(Ani->GlobalTime * 4),
      MatrRotateZ(15)),
      MatrRotateX(Ani->GlobalTime * 0)),
      MatrTranslate(0, 0, -50)),
      MatrScale(0.5, 0.5, 0.5));
  AK1_GeomDraw(&Uni->Geom, Uni->Sh);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AK1_PrimDraw(&Uni->Pr, Uni->Sh);
} /* End of 'ak1_AnimPlanetRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_UnitPlanetCreate( VOID )
{
  ak1UNIT_Planet *Uni;

  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_Planet))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AK1_AnimPlanetInit;
  Uni->Close = (VOID *)AK1_AnimPlanetClose;
  Uni->Response = (VOID *)AK1_AnimPlanetResponse;
  Uni->Render = (VOID *)AK1_AnimPlanetRender;
  return (ak1UNIT *)Uni;
} /* End of 'ak1_UnitBallCreate' function */

/* END OF 'UPLANETS.C' FILE */
