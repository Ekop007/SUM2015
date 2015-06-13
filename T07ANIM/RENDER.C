/* FILENAME: RENDER.C
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

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

typedef struct tagVERTEX
{
  VEC P;
  COLOR C;
}VERTEX;

UINT AK1_RndProg;

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
  FILE * F;
  VERTEX *V;
  INT (*Facets)[3];
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
  if ((V = malloc(sizeof(VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%f%f%f",
        &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].C = ColorSet(Rnd0(), Rnd0(), Rnd0(), 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      Facets[nf][0] = a - 1;
      Facets[nf][1] = b - 1;
      Facets[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;


  glGenVertexArrays(1, &GObj->VA);
  glGenBuffers(1, &GObj->VBuf);
  glGenBuffers(1, &GObj->IBuf);


  glBindVertexArray(GObj->VA);

  glBindBuffer(GL_ARRAY_BUFFER, GObj->VBuf);

  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * GObj->NumOfV, V, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GObj->IBuf);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT [3]) * GObj->NumOfF, Facets, GL_STATIC_DRAW);

  
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)0); 
  glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)sizeof(VEC)); 


  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);


  free(V);
  return TRUE;
} /* End of 'AK1_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ak1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjDraw( ak1GOBJ *GObj )
{
  INT loc;

  AK1_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(AK1_RndMatrWorld, AK1_RndMatrView), AK1_RndMatrProj);

  glLoadMatrixf(AK1_RndMatrWorldViewProj.A[0]);


  glBindVertexArray(GObj->VA);
  glUseProgram(AK1_RndProg);

  loc = glGetUniformLocation(AK1_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AK1_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(AK1_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AK1_RndMatrView.A[0]);
  loc = glGetUniformLocation(AK1_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AK1_RndMatrProj.A[0]);
  loc = glGetUniformLocation(AK1_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, AK1_Anim.Time);


  glDrawElements(GL_TRIANGLES, GObj->NumOfF * 3, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'AK1_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ak1GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjFree( ak1GOBJ *GObj )
{

  glBindVertexArray(GObj->VA);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &GObj->VBuf);
  glDeleteBuffers(1, &GObj->IBuf);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &GObj->VA);

  memset(GObj, 0, sizeof(ak1GOBJ));
} /* End of 'AK1_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */

