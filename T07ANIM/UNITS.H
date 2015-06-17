/* FILENAME: UNITS.H
 * PROGRAMMER: ak1
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

extern MATR AK1_ModelMatr;
extern MATR AK1_PlanetMatr;


ak1UNIT * AK1_UnitModelCreate( VOID );

ak1UNIT * AK1_UnitControlCreate( VOID );

ak1UNIT * AK1_UnitPlanetCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
