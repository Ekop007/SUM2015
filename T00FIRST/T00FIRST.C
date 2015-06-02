#include <stdio.h>
#include <windows.h>

void main( void )
{
  printf("Hello!!\n");
  if (MessageBox(NULL, "Yes or No", "Your answer?", MB_YESNO | MB_ICONQUESTION) == IDYES)
    MessageBox(NULL, "You press Yes!\n", "Answer", MB_OK | MB_ICONINFORMATION);
  else
    MessageBox(NULL, "You press NO!\n", "Answer", MB_OK | MB_ICONINFORMATION);
}