#include <stdio.h>

int split(char *str, char *ret[], char sep, int max)
{
  int i = 0;
  int c = 0;

  ret[0] = str;  /*ret[0]にstrの先頭アドレスを代入*/

  for(i = 0; *(str + i) != '\0'&& c < max; i++)
    {
      if(*(str + i) == sep)/*(str + i)がsepのとき*/
	{
	  *(str + i) = '\0';/*(str + i)にNULLを代入*/
	  c++;
	  ret[c] = str + (i + 1);/*ret[c]にNULL文字の次のアドレスを代入*/
	}
    }
  return c;
}

int main(void)
{
  char str[100] = {0};
  char *ret[10];
  char sep = ',';
  int max = 5;
  int i;
  int c;
  
  printf("Please input str.\n");
  scanf("%s", str);

  c = split(str, ret, sep, max);

  for(i = 0;i < c; i++)
    {
      printf("%d:%s\n", i + 1, ret[i]);
    }
  return 0;
}
