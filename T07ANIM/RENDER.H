/* FILENAME: RENDER.H
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include <stdio.h>
#include "vec.h"
 
#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
 
/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagak1GOBJ
{
  INT
    VA,         /* массив вершин объекта */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfV,  /* количество вершин */
    NumOfF;  /* количество граней */
} ak1GOBJ;

/* Матрицы */
extern MATR
  AK1_RndMatrWorld,
  AK1_RndMatrView,
  AK1_RndMatrProj;

extern MATR AK1_RndMatrWorldViewProj;
extern MATR AK1_ModelMatr;

/* Параметры проецирования */
extern DBL
  AK1_RndWp, AK1_RndHp,    /* размеры обрасти проецирования */
  AK1_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдера */
extern UINT AK1_RndProg, AK1_RndPlanet;


/* Матрица изменения примитива при создании */
extern MATR AK1_RndPrimMatrConvert;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT AK1_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_ShaderFree( UINT PrgId );

/***
 * Работа с примитивами
 ***/

/* Тип задания материала поверхности */
typedef struct tagak1MATERIAL
{
  /* Имя материала */
  CHAR Name[300];

  /* Коэффициенты отражения */
  VEC Ka, Kd, Ks;
  FLT Kp; /* Степень "бликовости" */

  /* Коэффициент прозрачности */
  FLT Kt;

  /* Текстура поверхности */
  INT TexId;
} ak1MATERIAL;

/* Глобальная таблица материалов */
#define AK1_MATERIAL_MAX 300
extern INT AK1_MtlLibSize;
extern ak1MATERIAL AK1_MtlLib[AK1_MATERIAL_MAX];

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       vg4MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT AK1_MtlAdd( ak1MATERIAL *Mtl );

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT AK1_MtlFind( CHAR *Name );

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT AK1_MtlLoad( CHAR *FileName );


/* Виды примитивов */
typedef enum tagak1PRIM_TYPE
{
  AK1_PRIM_TRIMESH,  /* примитив - набор треугольников */
  AK1_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} ak1PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagak1PRIM
{
  ak1PRIM_TYPE Type; /* Тип примитива (AK1_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI,
    MtlNo;      /* количество индексов для вывода примитива */
} ak1PRIM;

/* Тип хранения текстурных координат */
typedef struct tagak1UV
{
  FLT U, V; /* текстурные координаты */
} ak1UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UV) построенные координаты.
 */
__inline ak1UV UVSet( FLT U, FLT V )
{
  ak1UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagak1VERTEX
{
  VEC P;    /* позиция вершины */
  ak1UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} ak1VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ak1PRIM *Prim;
 *   - тип примитива (AK1_PRIM_***):
 *       ak1PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       ak1VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_PrimCreate( ak1PRIM *Prim, ak1PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ak1VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ak1PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_PrimFree( ak1PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ak1PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_PrimDraw( ak1PRIM *Prim, INT Sh );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ak1PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AK1_PrimCreatePlane( ak1PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ak1PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AK1_PrimCreateSphere( ak1PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ak1PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AK1_PrimLoad( ak1PRIM *GObj, CHAR *FileName );

BOOL AK1_PrimCreatePlanet( ak1PRIM *Prim, VEC C, FLT R, FLT Scale, CHAR *FileName );


/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagak1GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  ak1PRIM *Prims;      /* массив примитивов */
} ak1GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 *   - указатель на добавляемый примитив:
 *       ak1PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT AK1_GeomAddPrim( ak1GEOM *G, ak1PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_GeomFree( ak1GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_GeomDraw( ak1GEOM *G, INT Sh );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ak1GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL AK1_GeomLoad( ak1GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT AK1_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */