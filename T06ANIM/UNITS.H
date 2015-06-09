/* FILENAME: UNITS.H
 * PROGRAMMER: ak1
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"


/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ak1UNIT *) указатель на созданный объект анимации.
 */
ak1UNIT * AK1_UnitBallCreate( VOID );

ak1UNIT * AK1_UnitCowCreate( VOID );

ak1UNIT * AK1_UnitControlCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
