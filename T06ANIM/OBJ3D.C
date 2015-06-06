/* FILENAME: LOADOBJ.C
 * PROGRAMMER: VG4
 * PURPOSE: Load 3D nmodel from *.OBJ files.
 * LAST UPDATE: 06.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

/* Global model data */

/* Vertex array */
VEC *ObjV; /* Vertex coordinates */
INT ObjNumOfV; /* Number of model vertices */

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &ObjV[nv].X, &ObjV[nv].Y, &ObjV[nv].Z);
      nv++;
    }
  }

  fclose(F);

  ObjNumOfV = nv;
  return TRUE;
} /* End of 'ObjLoad' function */

/* Draw object functioln.
 * ARGUMENTS: 
 *       - hDC,
 *       - W, H/
 * RETURNS: None.
 */
VOID ObjDraw( HDC hMemDC, INT W, INT H )
{
  INT i;
  DBL x = sin(30);
  for (i = 0; i < ObjNumOfV; i++)
  {
    /* рисуем точку ObjV[i] */
    Ellipse(hMemDC, W / 2 + ObjV[i].X - 4, H / 2 + ObjV[i].Y + 4, W / 2 + ObjV[i].X + 4, H / 2 + ObjV[i].Y - 4);

  }

} /* End of 'ObjDraw' function */

/* END OF 'LOADOBJ.C' FILE */
