/* FILENAME: UCONTRL.C
 * PROGRAMMER: VG4
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagak1UNIT_CTRL
{
  AK1_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} ak1UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitInit( ak1UNIT_CTRL *Uni, ak1ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'AK1_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitClose( ak1UNIT_CTRL *Uni, ak1ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'AK1_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitResponse( ak1UNIT_CTRL *Uni, ak1ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AK1_AnimDoExit();
} /* End of 'AK1_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitRender( ak1UNIT_CTRL *Uni, ak1ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  RECT rc;
  static CHAR Buf[1000];

  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);

  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);

  SelectObject(Ani->hDC, hFntOld);
} /* End of 'AK1_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
ak1UNIT * AK1_UnitControlCreate( VOID )
{
  ak1UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AK1_AnimUnitCreate(sizeof(ak1UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AK1_AnimUnitInit;
  Uni->Close = (VOID *)AK1_AnimUnitClose;
  Uni->Response = (VOID *)AK1_AnimUnitResponse;
  Uni->Render = (VOID *)AK1_AnimUnitRender;
  return (ak1UNIT *)Uni;
} /* End of 'AK1_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
