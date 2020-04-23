#include <stdio.h>

int subst(char *str, char c1, char c2)
{
  int i;
  int c = 0;
  for(i = 0; *(str + i) != '\0'; i++)
    {
      if(*(str + i) == c1)
	{
	  *(str + i) = c2;
	  c++;
	}
    }
  return c;
}

int main(void)
{
  char str[100] = {0};
  char c1 = 0;
  char c2 = 0;
  char dummy;
  int c = 0; 

  printf("Input str.\n");
  scanf("%s", str);
  printf("Input c1.\n");
  scanf("%c", &dummy);
  scanf("%c", &c1);
  printf("Input c2.\n");
  scanf("%c", &dummy);
  scanf("%c", &c2);

  c = subst(str, c1, c2);
  
  printf("\nstr:%s\ncount:%d\n", str, c);

  return 0;
}
