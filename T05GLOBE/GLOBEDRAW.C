#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "GLOBE.H"
#include "IMAGE.H"

#define N 400
#define M 800

VEC Grid[N][M];
BYTE *Pic;
INT W, H;


/* Функция поворота точки вокруг оси X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * Pi / 180, si = sin(a), co = cos(a);
  VEC r;
  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* Функция векторного произведения */
/*VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

/* Рисование четырехугольника */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  POINT pnts[4];

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;

  if ((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
      (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
      (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
      (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
    return;


  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j, x, y;
  IMAGE Img;
  SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  if (Img.hBm == NULL)
    if (ImageLoad(&Img, "globe1.bmp") == FALSE)
      return;
  for (i = 0; i <= N; i++)
  {
    for (j = 0; j <= M; j++)
    {
      x = j * Img.W / (M + 1);
      y = i * Img.H / (N + 1);
   //   SetDCBrushColor(hDC, RGB(Pic[(y * PicW + x) * 3 + 2], Pic[(y * PicW + x) * 3 + 1], Pic[(y * PicW + x) * 3]));
      SetDCBrushColor(hDC, ImageGetP(&Img, x, y));
      DrawQuad(hDC, Grid[i][j], Grid[i][j + 1], Grid[i + 1][j + 1], Grid[i + 1][j], W, H);
    }
  }
  ImageFree(&Img);
}

VOID GlobeBild ( VOID )
{
  INT i, j, R = 200;
  DOUBLE phi, theta, tm = clock() / (DOUBLE)CLOCKS_PER_SEC;
  for (i = 0; i < N; i++)
  {
    theta = i * Pi / (N - 1.0);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * Pi / (M - 1.0) + tm * 0.3;
      Grid[i][j].X = sin(theta) * sin(phi) * R;
      Grid[i][j].Y = cos(theta) * R * 0.88;
      Grid[i][j].Z = sin(theta) * cos(phi) * R;
      Grid[i][j] = RotateX(Grid[i][j], 45 * sin(tm));
    }
  }
}