#include <stdio.h>

int subst(char *str, char c1, char c2)
{
  int i;                                                   /*forループ用*/
  int c = 0;                                               /*置き換えた文字数のカウント用*/
  for(i = 0; *(str + i) != '\0'; i++)                      /*入力文字列の終端に辿り着くまでループ*/
    {
      if(c1 == c2) break;                                  /*見た目上文字列に変化がないとき*/
      if(*(str + i) == c1)                                 /*(str + i)の文字がc1の文字と同じとき*/
	{
	  *(str + i) = c2;                                 /*(str + i)の文字をc2の文字に置き換える*/
	  c++;                                             /*置き換えた文字を数える*/
	}
    }
  return c;                                                /*置き換えた文字数を戻り値とする．*/
}

int main(void)
{
  FILE *fp;
  char filename[50];
  char buffer[10000];

  printf("Please indicate the filename.\n");

  if(fgets(filename, 50, stdin) == NULL)
    {
      printf("No filename.\n");
      return 1;
    }

  subst(filename, '\n', '\0');

  fp = fopen(filename, "r");

  if(fgets(buffer, 50, fp) == NULL)
    {
      printf("No filename.\n");
      return 1;
    }
  else
    {
      printf("buffer:%s\n", buffer);
    }

  fclose(fp);

  return 0;
}
