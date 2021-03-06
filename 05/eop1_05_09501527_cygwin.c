#include <stdio.h>
#include <string.h>                                        /*strncpy関数等用*/
#include <stdlib.h>                                        /*exit関数用*/

#define ESC 27                                             /*文字列ESCをESCのASCIIコードで置換*/
#define MAX_LINE 1025                                      /*文字配列LINEの最大入力数の指定用*/

/*構造体宣言*/
struct date
{
  int y; /*年*/
  int m; /*月*/
  int d; /*日*/
};

struct profile
{
  int id;               /*ID*/
  char name[70];        /*名前*/
  struct date birthday; /*誕生日(date構造体)*/
  char address[70];     /*住所*/
  char *biko;           /*備考*/
};

/*関数プロトタイプ宣言（煩雑化防止）*/
int subst(char *str, char c1, char c2);
int split(char *str, char *ret[], char sep, int max);
int get_line(char *line);
void parse_line(char *line);
void exec_command(char cmd, char *param);
void cmd_quit(void);
void cmd_check(void);
void cmd_print(char *param);
void cmd_read();
void cmd_write();
void cmd_find();
void cmd_sort();
void new_profile(struct profile *profile_p, char *line);

/*グローバル変数宣言*/
struct profile profile_data_store[10000];                  /*profile情報を格納*/
int profile_data_nitems = 0;                               /*profile情報の保存数を格納*/

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
  if(*line == ESC) cmd_quit();
  subst(line, '\n', '\0');                                 /*subst関数により，入力の改行文字を終端文字に置き換える*/
  return 1;                                                /*入力文字列が存在したとき，1を戻り値とする*/
}

void parse_line(char *line)
{
  char cmd;                                                /*%の次の1文字を格納用*/
  char *param;                                             /*コマンドのパラメータとなる文字列へのポインタ用*/

  if(*line == '%')                                         /*入力文字列の1文字目が%のとき*/
    {
      cmd = *(line + 1);                                   /*cmdに入力文字列の2文字目の値を代入*/
      param = line + 3;                                    /*paramにパラメータ部を代入*/
      exec_command(cmd, param);
    }
  else                                                     /*入力がコマンドではないとき*/
    {
      new_profile(&profile_data_store[profile_data_nitems++] ,line);
    }
}

void exec_command(char cmd, char *param)
{
  switch (cmd) {
  case 'Q': cmd_quit();   break;
    //case 'T': printf("Parameter test:\"%s\"\n", param); break;   /*ポインタparamの参照先から後ろに向かって，NULLまで文字列を表示する（デバッグ用）*/
  case 'C': cmd_check();  break;
  case 'P': cmd_print(param);  break;
  case 'R': cmd_read();   break;
  case 'W': cmd_write();  break;
  case 'F': cmd_find();   break;
  case 'S': cmd_sort();   break;
  default: fprintf(stderr, "Invalid command %c: ignored.\n", cmd); break;/*エラーメッセージを表示*/
  }
}

void cmd_quit()
{
  //char c;

  //while(1)
  //{
  //printf("Do you want to quit?(y/n)\n");               /*確認メッセージ*/
  //c = getchar();
  //getchar();                                           /*getcharでの入力時に改行文字が残ってしまうため*/
  //if(c == 'y')
  //{
  //printf("quit success.\n\n");
	  exit(0);
	  //}
	  //else if(c == 'n')
	  //{
	  //printf("quit cancelled.\n\n");
	  //break;
	  //}
	  //}
}

void cmd_check(void)
{
  printf("%d profile(s)\n", profile_data_nitems);
}

void cmd_print(char *param)
{
  int a = 0;
  int i = 0;                                                 /*forループ用*/

  a = atoi(param);                                           /*文字列をint型の値に変換*/

  /*aの絶対値がprofile_data_nitemsより大きいときかa=0のとき*/
  if(abs(a) >= profile_data_nitems|| a == 0) a = profile_data_nitems;

  if(a > 0)                                                  /*引数が正の整数のとき及び例外*/
    {
      for(i = 0; i < a; i++)
        {
	  printf("Id:    %d\n", profile_data_store[i].id);
	  printf("Name:  %s\n", profile_data_store[i].name);
	  printf("Birth: %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
	  printf("Addr:  %s\n",profile_data_store[i].address);
	  printf("Note:  %s\n\n",profile_data_store[i].biko);
	}
    }
  else if(a < 0)                                             /*引数が負の整数のとき*/
    {
      for(i = profile_data_nitems + a; i < profile_data_nitems; i++)
	{
	  printf("Id:    %d\n", profile_data_store[i].id);
	  printf("Name:  %s\n", profile_data_store[i].name);
	  printf("Birth: %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
	  printf("Addr:  %s\n",profile_data_store[i].address);
	  printf("Note:  %s\n\n",profile_data_store[i].biko);
	}
    }
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

void new_profile(struct profile *profile_p, char *line)
{
  char *ret[10];
  char *ret2[4];                                           /*誕生日の情報を分割し，その先頭アドレスを保存*/
  char sep = ',';                                          /*csvファイルからの入力を想定しているため，カンマ*/
  char sep2 = '-';                                         /*誕生日の入力文字列にあるハイフンで区切るため，ハイフン*/
  int max = 10;
  int max2 = 4;
  int c, birth_c;
  int MAX_BIKO = 0;                                        /*備考の文字数カウント用*/
  
  c = split(line, ret, sep, max);                          /*ID，名前などの情報を分割する*/
  if(c != 4)                                               /*入力形式が合わない場合*/
    {
      fprintf(stderr, "情報はID，名前，誕生日，住所，備考の順で入力される必要があります．\n処理を中止しました．\n\n");
      profile_data_nitems--;                               /*処理中止により，構造体に情報を書き込まないため*/
      return;
    }
  birth_c = split(ret[2], ret2, sep2, max2);               /*誕生日の年，月，日を分割する*/
  if(birth_c != 2)                                         /*誕生日の年，月，日を正常に分割できない場合*/
      {
	fprintf(stderr, "誕生日は\"年-月-日\"の形で入力される必要があります．\n処理を中止しました．\n\n"); /*年，月，日に分割できない場合，処理を停止*/
	profile_data_nitems--;                             /*処理中止により，構造体に情報を書き込まないため*/
	return;
      }

  /*構造体への情報の書き込み処理*/
  profile_p->id = atoi(ret[0]);                            /*IDの書き込み*/
  strncpy(profile_p->name, ret[1], 69);                    /*名前の書き込み*/
  (profile_p->birthday).y = atoi(ret2[0]);                 /*誕生年の書き込み*/
  (profile_p->birthday).m = atoi(ret2[1]);                 /*誕生月の書き込み*/
  (profile_p->birthday).d = atoi(ret2[2]);                 /*誕生日の書き込み*/
  strncpy(profile_p->address, ret[3], 69);                 /*住所の書き込み*/

  MAX_BIKO = strlen(ret[4]) + 1;                           /*備考情報の文字数のカウント*/

  profile_p->biko = (char *)malloc(sizeof(char) * MAX_BIKO); /*文字数分だけメモリ確保*/
  strncpy(profile_p->biko, ret[4], MAX_BIKO);              /*備考の書き込み*/
}

int main(void)
{
  char LINE[MAX_LINE] = {0};                               /*入力文字列(1行分)はmain関数で管理*/

  while(get_line(LINE))                                    /*文字配列LINEに文字列を入力する*/
    {
      parse_line(LINE);                                    /*入力文字列がある場合，構文解析を行う*/
    }
  return 0;
}
