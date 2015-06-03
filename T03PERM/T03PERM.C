/* T03PERM.C
 * Kachsheev Alexandr
 * spr02
 * Эта программа для счета транспозиций в массиве.
 */

#include <stdio.h>
#include <stdlib.h>

#define N 4

int Parity = 0;
int P[N];

void Swap( int *A, int *B )
{
 int tmp = *A;
 *A = *B;
 *B = tmp;
}

int CountParity( void )
{
  int i, j, cnt = 0;
  for (i = 0; i < N - 1; i++)
    for (j = i + 1; j < N; j++)
    {
      if(P[i] > P[j])
        cnt++;
    }
  return cnt;
}

void PrintToFile( int cnt )
{
  int i;
  FILE *F;
  F = fopen("Output.txt", "a");
  if (F != NULL)
  {
    for (i = 0; i < N; i++)
      fprintf(F, "%d", P[i]);
    fprintf(F, " %d - ", cnt);
    if (cnt % 2 == 0)
      fprintf(F, "%d чет\n", Parity);
    else
      fprintf(F, "%d нечет\n", Parity);
    fclose(F);
  }
}

void Go( int pos )
{
  int i, cnt = CountParity(), x, save;
  if (pos == N)
  {
    PrintToFile(cnt);
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

void CleanF( void )
{
  FILE *F;
  if ((F = fopen("Output.txt", "w")) != NULL)
    fclose(F);
}

void main( void )
{
  int i;
  for(i = 0; i < N; i++)
    P[i] = i + 1;
  CleanF();
  Go(0);
}