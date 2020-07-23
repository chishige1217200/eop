#include <stdio.h>
#include <string.h>

int main(void)
{
  char a[] = "561";
  char b[] = "79";
  char c[] = "56";
  char d[] = "079";

  printf("%d\n", strcmp(a, b));
  printf("%d\n", strcmp(c, b));

  printf("%d\n", strcmp(a, d));

  return 0;
}
