
#ifndef __GLOBE_H_
#define __GLOBE_H_

#include "DEF.H"

/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC; 

VOID GlobeBild ( VOID );
VOID GlobeDraw( HDC hDC, INT W, INT H );
#endif
