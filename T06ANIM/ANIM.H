/* FILENAME: ANIM.H
 * PROGRAMMER: AK1
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* ������������ ���������� ��������� �������� */
#define AK1_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagak1UNIT ak1UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  ak1UNIT *Units[AK1_MAX_UNITS]; 
  INT NumOfUnits;
  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */
  /* ���������� ����� */
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel,    /* ��������� ������ ���� */
    PosX, PosY;         /* ������� ����� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/
  DBL AngleX, AngleY;

} ak1ANIM;

/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT AK1_MouseWheel;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ak1UNIT *Unit, ak1ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ak1UNIT *Unit, ak1ANIM *Ani );
 */
#define AK1_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ak1UNIT *Unit, ak1ANIM *Ani );       \
  VOID (*Close)( ak1UNIT *Unit, ak1ANIM *Ani );      \
  VOID (*Response)( ak1UNIT *Unit, ak1ANIM *Ani );   \
  VOID (*Render)( ak1UNIT *Unit, ak1ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagak1UNIT
{
  AK1_UNIT_BASE_FIELDS;
};

extern ak1ANIM AK1_Anim;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AK1_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AK1_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AK1_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AK1_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AK1_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ak1UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AK1_AnimAddUnit( ak1UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AK1_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ak1UNIT *) ��������� �� ��������� ������ ��������.
 */
ak1UNIT * AK1_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AK1_AnimDoExit( VOID );

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AK1_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
