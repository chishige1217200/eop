#include <stdio.h>

int int_value_check(char *str)
{
  while(*str)
    {
      if(*str >= 48 && *str <= 57 ) str++;
      else return 0;
    }
  return 1;  
}

int main(void)
{
  char a[10] = "10AB";
  char b[10] = "999";

  if(int_value_check(a)) printf("\"%s\" has the int compatibility.\n", a);
  if(int_value_check(b)) printf("\"%s\" has the int compatibility.\n", b);

  return 0;
}
