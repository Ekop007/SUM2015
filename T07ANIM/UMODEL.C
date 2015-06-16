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

  ak1GEOM Model; 
  ak1GEOM Geom; 
  ak1PRIM Pr;
  INT TextId;     /* Модель корова */
} ak1UNIT_MODEL;

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
  ak1VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  Ani->AngleY = 0;
  Ani->AngleX = 0;
  Ani->PosX = 0;
  Ani->PosY = 0;
 // Uni->TextId = AK1_TextureLoad("M.BMP");

//  AK1_PrimCreate(&Uni->Pr, AK1_PRIM_TRIMESH, 4, 6, V, I);

//  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  
  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(0.010, 0.010, 0.010), MatrRotateY(0));
  AK1_GeomLoad(&Uni->Geom, "E:\\SPR02\\Helic\\RAS.G3D");
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
  AK1_GeomFree(&Uni->Model);
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
  if (Ani->JX > 0.00001)
  {
    Ani->PosX += 5 * (Ani->JX + 0.00002) * !Ani->IsPause;
    Ani->AngleX = 20 * (Ani->JX + 0.00002) * !Ani->IsPause;
  }
  if (Ani->JX < -0.00003)
  {
    Ani->PosX += 5 * (Ani->JX + 0.00002) * !Ani->IsPause;
    Ani->AngleX = 20 * (Ani->JX + 0.00002) * !Ani->IsPause;
  }
  if (Ani->JY > 0.00001)
  {
    Ani->PosY += 5 * (Ani->JY + 0.00002) * !Ani->IsPause;
    Ani->AngleY = 20 * (Ani->JY + 0.00002) * !Ani->IsPause;
  }
  if (Ani->JY < -0.00003)
  {
    Ani->PosY += 5 * (Ani->JY + 0.00002) * !Ani->IsPause;
    Ani->AngleY = 20 * (Ani->JY + 0.00002) * !Ani->IsPause;
  }
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
  AK1_RndMatrView = MatrView(VecSet(0, 2, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  AK1_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrRotateY(Ani->AngleX),
      MatrRotateX(Ani->AngleY)),
      MatrTranslate(/*Ani->PosX, -Ani->PosY*/ 0, 0, 0)),
      MatrScale(0.1, 0.1, 0.1));
  AK1_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AK1_PrimDraw(&Uni->Pr);
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
