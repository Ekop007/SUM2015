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
 
/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagak1GOBJ
{
  INT
    VA,         /* ������ ������ ������� */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfV,  /* ���������� ������ */
    NumOfF;  /* ���������� ������ */
} ak1GOBJ;

/* ������� */
extern MATR
  AK1_RndMatrWorld,
  AK1_RndMatrView,
  AK1_RndMatrProj;

extern MATR AK1_RndMatrWorldViewProj;
extern MATR AK1_ModelMatr;

/* ��������� ������������� */
extern DBL
  AK1_RndWp, AK1_RndHp,    /* ������� ������� ������������� */
  AK1_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� */
extern UINT AK1_RndProg;


/* ������� ��������� ��������� ��� �������� */
extern MATR AK1_RndPrimMatrConvert;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT AK1_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID AK1_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagak1MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} ak1MATERIAL;

/* ���������� ������� ���������� */
#define AK1_MATERIAL_MAX 300
extern INT AK1_MtlLibSize;
extern ak1MATERIAL AK1_MtlLib[AK1_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       vg4MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT AK1_MtlAdd( ak1MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT AK1_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT AK1_MtlLoad( CHAR *FileName );


/* ���� ���������� */
typedef enum tagak1PRIM_TYPE
{
  AK1_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  AK1_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} ak1PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagak1PRIM
{
  ak1PRIM_TYPE Type; /* ��� ��������� (AK1_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,
    MtlNo;      /* ���������� �������� ��� ������ ��������� */
} ak1PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagak1UV
{
  FLT U, V; /* ���������� ���������� */
} ak1UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (ak1UV) ����������� ����������.
 */
__inline ak1UV UVSet( FLT U, FLT V )
{
  ak1UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagak1VERTEX
{
  VEC P;    /* ������� ������� */
  ak1UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} ak1VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ak1PRIM *Prim;
 *   - ��� ��������� (AK1_PRIM_***):
 *       ak1PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       ak1VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID AK1_PrimCreate( ak1PRIM *Prim, ak1PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ak1VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ak1PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AK1_PrimFree( ak1PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ak1PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AK1_PrimDraw( ak1PRIM *Prim, UINT Unt );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ak1PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AK1_PrimCreatePlane( ak1PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       ak1PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AK1_PrimCreateSphere( ak1PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ak1PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AK1_PrimLoad( ak1PRIM *GObj, CHAR *FileName );

BOOL AK1_PrimCreatePlanet( ak1PRIM *Prim, VEC C, FLT R, FLT Scale, CHAR *FileName );


/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagak1GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  ak1PRIM *Prims;      /* ������ ���������� */
} ak1GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ak1GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       ak1PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT AK1_GeomAddPrim( ak1GEOM *G, ak1PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ak1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AK1_GeomFree( ak1GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ak1GEOM *G;
 * ������������ ��������: ���.
 */
VOID AK1_GeomDraw( ak1GEOM *G, UINT Unt );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ak1GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL AK1_GeomLoad( ak1GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT AK1_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */