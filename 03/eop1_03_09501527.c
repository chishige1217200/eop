#include <stdio.h>
#include <stdlib.h>                                        /*exit関数用*/

#define ESC 27                                             /*文字列ESCをESCのASCIIコードで置換*/
#define MAX_LINE 1025                                      /*文字配列LINEの最大入力数の指定用*/

/*関数プロトタイプ宣言（煩雑化防止）*/
int subst(char *str, char c1, char c2);
int split(char *str, char *ret[], char sep, int max);
int get_line(char *line);
void parse_line(char *line);
void exec_command(char cmd, char *param);
void cmd_quit();
void cmd_check();
void cmd_print();
void cmd_read();
void cmd_write();
void cmd_find();
void cmd_sort();
void new_profile(char *line);

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

  for(i = 0; *(str + i) != '\0'&& c < max; i++)            /*cがmaxより小さいかつ入力文字列の終端に辿り着いていないときループ*/
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
  if(*line == ESC) return 0;                               /*直接入力のとき，入力文字列を空にできないため，ESCキーの単独入力により0を戻り値とする（デバッグ用）*/
  subst(line, '\n', '\0');                                 /*subst関数により，入力の改行文字を終端文字に置き換える*/
  return 1;                                                /*入力文字列が存在したとき，1を戻り値とする*/
}

void parse_line(char *line)
{
  char cmd;                                                /*%の次の1文字を格納用*/
  char *param;                                             /*コマンドのパラメータとなる文字列へのポインタ用*/
  char *buffer = "(Null Parameter)";                       /*例外処理用*/

  if(*line == '%')                                         /*入力文字列の1文字目が%のとき*/
    {
      cmd = *(line + 1);                                   /*cmdに入力文字列の2文字目の値を代入*/
      if(*(line + 3) != '\0') param = (line + 3);          /*ポインタlineに3を足したアドレスをポインタparamに代入*/
      else param = buffer;                                 /*入力文字列にパラメータ部が無いとき，文字列"(Null Parameter)"のアドレスをポインタparamに代入*/
      exec_command(cmd, param);
    }
  else                                                     /*入力がコマンドではないとき*/
    {
      new_profile(line);
    }
}

void exec_command(char cmd, char *param)
{
  switch (cmd) {
  case 'Q': cmd_quit();   break;
  case 'T': printf("Parameter test:\"%s\"\n", param); break;   /*ポインタparamの参照先から後ろに向かって，NULLまで文字列を表示する（デバッグ用）*/
  case 'C': cmd_check();  break;
  case 'P': cmd_print();  break;
  case 'R': cmd_read();   break;
  case 'W': cmd_write();  break;
  case 'F': cmd_find();   break;
  case 'S': cmd_sort();   break;
  default: fprintf(stderr, "%%%c command is invoked with arg: \"%s\"\n", cmd, param); break;/*エラーメッセージを表示*/
  }
}

void cmd_quit()
{
  printf("Do you want to quit?(y/n)\n");                   /*確認メッセージ*/
  if(getchar() == 'y')
    {
      printf("quit success.\n");
      exit(0);
    }
  getchar();                                               /*getcharでの入力時に改行文字が残ってしまうため*/
  printf("quit cancelled.\n");
}

void cmd_check()
{
  fprintf(stderr, "Check command is invoked.\n");
}

void cmd_print()
{
  fprintf(stderr, "Print command is invoked.\n");
}

void cmd_read()
{
  fprintf(stderr, "Read command is invoked.\n");
}

void cmd_write()
{
  fprintf(stderr, "Write command is invoked.\n");
}

void cmd_find()
{
  fprintf(stderr, "Find command is invoked.\n");
}

void cmd_sort()
{
  fprintf(stderr, "Sort command is invoked.\n");
}

void new_profile(char *line)
{
  char *ret[11];
  char sep = ',';                                          /*csvファイルからの入力を想定しているため，カンマ*/
  int max = 10;
  int c, i;
  static int a = 1;                                        /*値をmain関数終了時まで保持する必要があるため，static int型*/
  
  printf("line number:%d\n", a);
  printf("input:\"%s\"\n", line);
  c = split(line, ret, sep, max);                          /*split関数を呼び出す*/
  for(i = 0;i <= c; i++)
    {
      printf("split[%d]:\"%s\"\n", i, ret[i]);
    }
  printf("\n");                                            /*見やすさのために改行*/
  a++;
}

int main(void)
{
  char LINE[MAX_LINE] = {0};                               /*入力文字列(1行分)はmain関数で管理*/

  while(get_line(LINE))                                    /*文字配列LINEに文字列を入力する(get_line関数)*/
    {
      parse_line(LINE);                                    /*入力文字列がある場合，構文解析を行う(parse_line関数)*/
    }
  return 0;
}
