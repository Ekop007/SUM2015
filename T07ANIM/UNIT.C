/* FILENAME: UNIT.C
 * PROGRAMMER: AK1
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitInit( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitClose( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitResponse( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ak1UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ak1ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AK1_AnimUnitRender( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ak1UNIT *) ��������� �� ��������� ������ ��������.
 */
ak1UNIT * AK1_AnimUnitCreate( INT Size )
{
  ak1UNIT *Uni;

  if (Size < sizeof(ak1UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = AK1_AnimUnitInit;
  Uni->Close = AK1_AnimUnitClose;
  Uni->Response = AK1_AnimUnitResponse;
  Uni->Render = AK1_AnimUnitRender;
  return Uni;
} /* End of 'ak1_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
