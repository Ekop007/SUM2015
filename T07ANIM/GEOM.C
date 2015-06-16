/* FILENAME: GEOM.C
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system implementation module.
 * LAST UPDATE: 14.06.2015
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

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(ak1PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(ak1PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'AK1_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /* посылаем количество частей */
  glUseProgram(AK1_RndProg);
  loc = glGetUniformLocation(AK1_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  /* рисуем непрозрачные объекты */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (AK1_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      /* посылаем номер текущей части */
      glUseProgram(AK1_RndProg);
      loc = glGetUniformLocation(AK1_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      AK1_PrimDraw(&G->Prims[i]);
    }

  /* рисуем прозрачные объекты */
  for (i = 0; i < G->NumOfPrimitives; i++)
    if (AK1_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      /* посылаем номер текущей части */
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
  static CHAR
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  memset(G, 0, sizeof(ak1GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(AK1_RndPrimMatrConvert));

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем и загружаем библиотеку материалов */
  _makepath(path_buffer, drive, dir, MtlName, "");
  AK1_MtlLoad(path_buffer);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    ak1VERTEX *Vert;
    ak1PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(ak1VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(ak1VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = VecMulMatr(Vert[j].P, AK1_RndPrimMatrConvert);
      Vert[j].N = PointTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    AK1_PrimCreate(&P, AK1_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = AK1_MtlFind(MtlName);

    free(Vert);

    /* добавляем примитив к объекту */
    AK1_GeomAddPrim(G, &P);
  }
  fclose(F);
  AK1_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'AK1_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
