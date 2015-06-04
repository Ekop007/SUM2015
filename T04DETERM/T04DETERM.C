/* T03PERM.C
 * Kachsheev Alexandr
 * spr02
 * Ёта программа дл€ счета определител€ в матрице.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXN 10

int N;
double M[MAXN][MAXN];
int P[MAXN];
int Parity = 0;
double SUM = 0;

void Swap( int *A, int *B )
{
 int tmp = *A;
 *A = *B;
 *B = tmp;
}    

void Go( int pos )
{
  int i, x, save, j;
  double Prod = 1;
  if (pos == N)
  {
    for (j = 0; j < N; j++)
    {
      Prod *= M[j][P[j]];
    }
    if (Parity == 0)
      SUM += Prod;
    else
      SUM -= Prod;
    return;
  }
  save = Parity;
  for (i = pos; i < N; i++)
  {
    Swap(&P[pos], &P[i]);
    if (pos != i)
      Parity = !Parity;
    Go(pos + 1);     
  }
  x = P[pos];
  Parity = save;
  for (i = pos +1; i < N; i++)
     P[i - 1] = P[i];
  P[N - 1] = x;
}

void WriteToF( void )
{
  FILE *F;
  if ((F = fopen("Output.txt", "a")) != NULL)
  {
    fprintf(F, " %lf\n", SUM);
    fclose(F);
  }
}

void ScanFromFile( FILE *F )
{
  int i, j;
  if (F != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &M[i][j]);
  }
}

void main( void )
{
  int i, j;
  FILE *F;
  F = fopen("Output.txt", "r");
  ScanFromFile(F);
  for(i = 0; i < N; i++)
  {
    P[i] = i;
  }
  Go(0);
  WriteToF();
  fclose(F);
}