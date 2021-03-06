/* FILENAME: RENDER.H
 * PROGRAMMER: AK1
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"
 
 
/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagak1GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} ak1GOBJ;

/* ������� */
extern MATR
  AK1_RndMatrWorld,
  AK1_RndMatrView,
  AK1_RndMatrProj;

/* ��������� ������������� */
extern DBL
  AK1_RndWp, AK1_RndHp,    /* ������� ������� ������������� */
  AK1_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT AK1_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AK1_RndGObjLoad( ak1GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AK1_RndGObjFree( ak1GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AK1_RndGObjDraw( ak1GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
