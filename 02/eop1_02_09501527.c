#include <stdio.h>

#define ESC 27                                             /*文字列ESCをESCのASCIIコードで置換*/
#define MAX_LINE 1025                                      /*文字配列LINEの最大入力数の指定用*/

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

int split(char *str, char *ret[], char sep, int max)
{
  int i;                                                   /*forループ用*/
  int c = 0;                                               /*ポインタの配列の指定用*/

  ret[0] = str;                                            /*ret[0]にstrの先頭アドレスを代入*/

  for(i = 0; *(str + i) != '\0'&& c < max; i++)            /*cがmaxより小さいか，入力文字列の終端に辿り着いていないときループ*/
    {
      if(*(str + i) == sep)                                /*(str + i)がsepのとき*/
	{
	  *(str + i) = '\0';                               /*(str + i)にNULLを代入*/
	  c++;
	  ret[c] = str + (i + 1);                          /*ret[c]にNULL文字の"次の"アドレスを代入*/
	}
    }
  return c;                                                /*文字列をいくつに分割したかを戻り値とする*/
}

int get_line(char *line)
{
  if(fgets(line, MAX_LINE, stdin) == NULL) return 0;       /*入力文字列が空のとき，0を戻り値とする．入力文字列は1024文字*/
  if(*line == ESC) return 0;                               /*直接入力のとき，入力文字列を空にできないため，ESCキーの単独入力により0を戻り値とする*/
  subst(line, '\n', '\0');                                 /*subst関数により，入力の改行文字を終端文字に置き換える*/
  return 1;                                                /*入力文字列が存在したとき，1を戻り値とする*/
}

int main(void)
{
  char line[MAX_LINE] = {0};                               /*入力文字列は最大1024文字*/
  char *ret[11];
  char sep = ',';                                          /*csvファイルからの入力を想定しているため，カンマ*/
  int max = 10;
  int c, i, a = 1;
  
  while(get_line(line))                                    /*get_line関数を呼び出し，戻り値が0ならループを終了*/
    {
      printf("line number:%d\n", a);
      printf("input:\"%s\"\n", line);
      c = split(line, ret, sep, max);                      /*split関数を呼び出す*/
      for(i = 0;i <= c; i++)
	{
	  printf("split[%d]:\"%s\"\n", i, ret[i]);
	}
      printf("\n\n");                                      /*見やすさのために改行*/
      a++;
    }
  return 0;
}
