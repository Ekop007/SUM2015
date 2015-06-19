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

typedef struct tagak1UNIT_Planet
{
  AK1_UNIT_BASE_FIELDS;

  ak1GEOM Geom; 
  ak1PRIM Pr;
  INT TextId;
  FLT Angle;
  VEC Pos[4];
  UINT ProgId[4];
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
  Uni->ProgId[0] = 1;
  Uni->ProgId[1] = 1;
  Uni->Angle = Ani->GlobalTime * 0.5;
  Uni->Geom.Prims = (ak1PRIM *)malloc(sizeof(ak1PRIM) * 4);
  Uni->Pos[0] = VecSet(0, 0, 0);
  AK1_PrimCreateSphere(&Uni->Geom.Prims[0], Uni->Pos[0], 600, 500, 500);
  Uni->Geom.NumOfPrimitives++;
  Uni->ProgId[0] = AK1_ShaderLoad("AXIS");
  Mtl.TexId = AK1_TextureLoad("olok.bmp");
  Uni->Geom.Prims[0].MtlNo = AK1_MtlAdd(&Mtl);
  
  
  //  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  
 // AK1_RndPrimMatrConvert = MatrMulMatr(MatrMulMatr(MatrTranslate(0, 0, 0), MatrScale(20, 0.020, 0.020)), MatrRotateY(90));
//  &Uni->Geom.Prims = (ak1PRIM *)malloc(sizeof(ak1PRIM));
  Uni->Pos[1] = VecSet(0, -150, 210);
  AK1_PrimCreatePlanet(&Uni->Geom.Prims[1], VecSet(0, 0, 0), 100, 5, "yptb.bmp");
  Mtl.TexId = AK1_TextureLoad("yptc.bmp");
  Uni->ProgId[1] = AK1_ShaderLoad("PLANET");
  Uni->Geom.Prims[1].MtlNo = AK1_MtlAdd(&Mtl);

  Uni->Pos[2] = VecSet(-150, -50, -170);
  AK1_PrimCreatePlanet(&Uni->Geom.Prims[2], VecSet(0, 0, 0), 100, 5, "mars1.bmp");
  Mtl.TexId = AK1_TextureLoad("mars.bmp");
  Uni->Geom.Prims[2].MtlNo = AK1_MtlAdd(&Mtl);

  Uni->Pos[3] = VecSet(190, -190, -20);
  AK1_PrimCreatePlanet(&Uni->Geom.Prims[3], VecSet(0, 0, 0), 100, 5, "blabl.bmp");
  Mtl.TexId = AK1_TextureLoad("blab.bmp");
  Uni->Geom.Prims[3].MtlNo = AK1_MtlAdd(&Mtl);

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
  INT i;
  AK1_RndMatrView = MatrView(VecSet(0, 2, -3), VecSet(0, 0, 0), VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  
  AK1_RndMatrWorld = MatrRotateY(40 * Ani->GlobalTime);

  for (i = 0; i < 4; i ++)
  { 
    AK1_PlanetMatr =
      MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(
        MatrRotateY(Ani->GlobalTime * 1),
        MatrRotateZ(15)),
        MatrRotateX(Ani->GlobalTime * 0)),
        MatrTranslate(Uni->Pos[i].X, Uni->Pos[i].Y, Uni->Pos[i].Z)),
        MatrScale(1, 1, 1));
    glActiveTexture(GL_TEXTURE0);
    if (i > 1)
    {
      glBindTexture(GL_TEXTURE_2D, Uni->Geom.Prims[i].MtlNo);
      AK1_PrimDraw(&Uni->Geom.Prims[i], Uni->ProgId[1]);
    }
    else
    {
      glBindTexture(GL_TEXTURE_2D, Uni->Geom.Prims[i].MtlNo);
      AK1_PrimDraw(&Uni->Geom.Prims[i], Uni->ProgId[i]);
    }
  }
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
