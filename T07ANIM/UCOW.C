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

  ak1GEOM Model; 
  ak1GEOM Geom; 
  ak1PRIM Pr;
  INT TextId;     /* Модель корова */
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

 
  Uni->TextId = AK1_TextureLoad("M.BMP");

  AK1_PrimCreate(&Uni->Pr, AK1_PRIM_TRIMESH, 4, 6, V, I);

  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  

  AK1_RndPrimMatrConvert = MatrMulMatr(MatrScale(3, 3, 3), MatrRotateX(-90));
  AK1_GeomLoad(&Uni->Geom, "X6.G3D");
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
  AK1_GeomFree(&Uni->Model);
  AK1_GeomFree(&Uni->Geom);
  AK1_PrimFree(&Uni->Pr);
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
  INT i, j;

  AK1_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  AK1_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrTranslate(Ani->PosX, -Ani->PosY, 0),
      MatrScale(0.1, 0.1, 0.1)),
      MatrRotateY(Ani->AngleY)),
      MatrRotateX(Ani->AngleX));
  AK1_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AK1_PrimDraw(&Uni->Pr);
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
