/* FILENAME: UBALL.C
 * PROGRAMMER: AK6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagak1UNIT_BALL
{
  AK1_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    Amplitude,  /* ��������� */
    PhaseShift, /* ����� �� ���� */
    ScaleShift; /* ������� ������� */
} ak1UNIT_BALL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitInit( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AK1_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitClose( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
} /* End of 'AK1_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitResponse( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
} /* End of 'AK1_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1NIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitRender( ak1UNIT_BALL *Uni, ak1ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time * 300 / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'AK1_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (ak1UNIT *) ��������� �� ��������� ������ ��������.
 */
ak1UNIT * AK1_UnitBallCreate( VOID )
{
  ak1UNIT_BALL *Uni;
  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_BALL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AK1_AnimUnitInit;
  Uni->Close = (VOID *)AK1_AnimUnitClose;
  Uni->Response = (VOID *)AK1_AnimUnitResponse;
  Uni->Render = (VOID *)AK1_AnimUnitRender;
  return (ak1UNIT *)Uni;
} /* End of 'AK1_UnitBallCreate' function */


/* END OF 'UBALL.C' FILE */
