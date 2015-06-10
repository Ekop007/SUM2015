/* FILENAME: VEC.H
 * PROGRAMMER: AK1
 * PURPOSE: Vectors and matrix system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef _VEC_H_
#define _VEC_H_

#include <math.h>

#include "def.h"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)
#define DEGREE2RADIANL1 0.01745329251994329576L
#define AK1_UNIT_MATR \
{                     \
  {                   \
    {1, 0, 0, 0},     \
    {0, 1, 0, 0},     \
    {0, 0, 1, 0},     \
    {0, 0, 0, 1}      \
  }                   \
}

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR M;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
}

__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR S =
  {
    {
      {Sx, 0, 0, 0},
      {0, Sy, 0, 0},
      {0, 0, Sz, 0},
      {0, 0, 0, 1}
    }
  };
  return S;
}

__inline MATR MatrTranspose( MATR M )
{
  MATR MT =
  {
    {
      {M.A[0][0], M.A[1][0], M.A[2][0], M.A[3][0]},
      {M.A[0][1], M.A[1][1], M.A[2][1], M.A[3][1]},
      {M.A[0][2], M.A[1][2], M.A[2][2], M.A[3][2]},
      {M.A[0][3], M.A[1][3], M.A[2][3], M.A[3][3]}
    }
  };
  return MT;
}

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}



__inline DBL MatrDeterm( MATR M )
{
  return  
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
}

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

__inline VEC VecMulNum( VEC V, DBL N )
{
  return VecSet(V.X * N, V.Y * N, V.Z * N); 
}

__inline VEC VecDivNum( VEC V, DBL N )
{
  return VecSet(V.X / N, V.Y / N, V.Z / N);
}

__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}

__inline VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
}

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen( VEC V )
{
  return sqrt(VecLen2(V));
}

__inline VEC VecNormalize( VEC V )
{
  DBL Len = VecLen(V);
  if (Len != 0 && Len != 1)
  {
    V.X = V.X / Len;
    V.Y = V.Y / Len;
    V.Z = V.Z / Len;
  }
  return V;
}


__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VecMulMatr( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

__inline VEC NormVec( VEC N, MATR M )
{
  VEC N1;
  MATR Q = MatrTranspose(MatrInverse(M));
  N1 = VecMulMatr(N, Q);
  return N1;
}

__inline MATR MatrRotateZ( MATR M, DBL AngleInDegree )
{
  DBL sine, cosine;
  /* Multiplyer degree to radian constant */
   long double MultiplierDegree2Radian = DEGREE2RADIANL1;
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[1][1] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;
  return M;
} /* End of 'MatrRotateZ' function */

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  DBL sine, cosine;
  /* Multiplyer degree to radian constant */
  long double MultiplierDegree2Radian = DEGREE2RADIANL1;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[0][0] =  M.A[2][2] = cosine;
  M.A[2][0] = sine;
  M.A[0][2] = -sine;
  return M;
} /* End of 'VecRotateY' function */

__inline MATR MatrRotateX( DBL AngleInDegree )
{                        
  MATR M = MatrIdentity();
  DBL sine, cosine;
  /* Multiplyer degree to radian constant */
  long double MultiplierDegree2Radian = DEGREE2RADIANL1;
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  M.A[1][1] =  M.A[0][0] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;
  return M;
} /* End of 'VecRotateX' function */

__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  MATR M = MatrIdentity();
  VEC V = {X, Y, Z};
  DBL sine, cosine, len = VecLen(V);
  /* Multiplyer degree to radian constant */
  long double MultiplierDegree2Radian = DEGREE2RADIANL1;
  
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld AngleDegree 
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  if (len != 0 && len != 1)
  {
    X = V.X / len; 
    Y = V.Y / len;
    Z = V.Z / len;
  }

  /* Build quaternion matrix */
  X *= sine;
  Y *= sine;
  Z *= sine;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  M.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  M.A[2][1] = 2 * cosine * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
}

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR M =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.X, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return M;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR M =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return M;
} /* End of 'MatrFrust' function */

#endif