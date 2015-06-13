/* FILENAME: RENDER.H
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

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

/* Параметры проецирования */
extern DBL
  AK1_RndWp, AK1_RndHp,    /* размеры обрасти проецирования */
  AK1_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT AK1_RndProg;

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT AK1_RndWorldToScreen( VEC P );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AK1_RndGObjLoad( ak1GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjFree( ak1GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AK1_RndGObjDraw( ak1GOBJ *GObj );

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

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
