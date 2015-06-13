/* FILENAME: UNIT.C
 * PROGRAMMER: AK1
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitInit( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitClose( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitResponse( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ak1UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ak1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AK1_AnimUnitRender( ak1UNIT *Uni, ak1ANIM *Ani )
{
} /* End of 'ak1_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_AnimUnitCreate( INT Size )
{
  ak1UNIT *Uni;

  if (Size < sizeof(ak1UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля по-умолчанию */
  Uni->Size = Size;
  Uni->Init = AK1_AnimUnitInit;
  Uni->Close = AK1_AnimUnitClose;
  Uni->Response = AK1_AnimUnitResponse;
  Uni->Render = AK1_AnimUnitRender;
  return Uni;
} /* End of 'ak1_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
