/* FILENAME: RENDER.C
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* Матрицы */
MATR
  AK1_RndMatrWorld = AK1_UNIT_MATR,
  AK1_RndMatrView = AK1_UNIT_MATR,
  AK1_RndMatrProj = AK1_UNIT_MATR,
  AK1_RndMatrWorldViewProj = AK1_UNIT_MATR;

/* Параметры проецирования */
DBL
  AK1_RndWp = 3, AK1_RndHp = 3,     /* размеры обрасти проецирования */
  AK1_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT AK1_RndWorldToScreen( VEC P )
{
  POINT Ps;

  /* преобразование СК */
  P = PointTransform(P, AK1_RndMatrWorldViewProj);

  Ps.x = (P.X + 1) / 2 * AK1_Anim.W + AK1_Anim.PosX;
  Ps.y = (1 - P.Y) / 2 * AK1_Anim.H + AK1_Anim.PosY;

  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */


/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AK1_RndGObjLoad( ak1GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(ak1GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'VG4_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjDraw( ak1GOBJ *GObj )
{
  INT i;
  POINT *pnts;
    
  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;
  /* проецируем все точки */
  AK1_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(AK1_RndMatrWorld, AK1_RndMatrView), AK1_RndMatrProj);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = AK1_RndWorldToScreen(GObj->V[i]);
  
  /* рисуем треугольники */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];

    MoveToEx(AK1_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(AK1_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(AK1_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(AK1_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'VG4_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjFree( ak1GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(ak1GOBJ));
} /* End of 'VG4_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */

