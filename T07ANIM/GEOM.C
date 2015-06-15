/* FILENAME: GEOM.C
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 *   - указатель на добавляемый примитив:
 *       ak1PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT AK1_GeomAddPrim( ak1GEOM *G, ak1PRIM *Prim )
{
  ak1PRIM *new_bulk;

  /**/
  if ((new_bulk = malloc(sizeof(ak1PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /**/
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(ak1PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /**/
  G->Prims = new_bulk;

  /**/
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'AK1_GeomAddPrim' function */

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический примитив:
 *       ak1GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: нет.
 */
VOID AK1_GeomFree( ak1GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      AK1_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(ak1GEOM));
} /* End of 'AK1_GeomFree' function */

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_GeomDraw( ak1GEOM *G )
{
  INT i, loc;

  /**/
  glUseProgram(AK1_RndProg);
  loc = glGetUniformLocation(AK1_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  for (i = 0; i < G->NumOfPrimitives; i++)
  {
    /**/
    glUseProgram(AK1_RndProg);
    loc = glGetUniformLocation(AK1_RndProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    glUseProgram(0);
    AK1_PrimDraw(&G->Prims[i]);
  }
} /* End of 'AK1_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL AK1_GeomLoad( ak1GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];

  memset(G, 0, sizeof(ak1GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(AK1_RndPrimMatrConvert));

  /**/
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /**/
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /**/
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    ak1VERTEX *Vert;
    ak1PRIM P;

    /**/
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /**/
    fread(MtlName, 1, 300, F);

    /**/
    if ((Vert = malloc(sizeof(ak1VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /**/
    fread(Vert, sizeof(ak1VERTEX), nv, F);
    /**/
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = VecMulMatr(Vert[j].P, AK1_RndPrimMatrConvert);
      Vert[j].N = PointTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /**/
    AK1_PrimCreate(&P, AK1_PRIM_TRIMESH, nv, ni, Vert, Ind);

    free(Vert);

    /**/
    AK1_GeomAddPrim(G, &P);
  }
  fclose(F);
  AK1_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'AK1_GeomDraw' function */
/* END OF 'GEOM.C' FILE */
