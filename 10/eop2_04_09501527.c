#include <stdio.h>
#include <string.h>                                          /*strncpy関数等用*/
#include <stdlib.h>                                          /*exit関数用*/

#define ESC 27                                               /*文字列ESCをESCのASCIIコードで置換*/
#define MAX_LINE 1025                                        /*文字配列LINEの最大入力数の指定用*/

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
int get_line(FILE *F, char *line);
void parse_line(char *line);
void exec_command(char *cmd, char *param);
void cmd_quit(char *param);
void cmd_check(void);
void cmd_print(char *param);
void cmd_read(char *param);
void cmd_write(char *param);
void cmd_find(char *param);
void cmd_sort(char *param);
void data_move(struct profile *sp1, struct profile *sp2);
void new_profile(struct profile *profile_p, char *line);
int int_value_check(char *str);
int input_format_check(char *str);

/*グローバル変数宣言*/
struct profile profile_data_store[10000];                    /*profile情報を格納*/
int profile_data_nitems = 0;                                 /*profile情報の保存数を格納*/

int subst(char *str, char c1, char c2)
{
  int i;                                                     /*forループ用*/
  int c = 0;                                                 /*置き換えた文字数のカウント用*/
  for(i = 0; *(str + i) != '\0'; i++)                        /*入力文字列の終端に辿り着くまでループ*/
    {
      if(c1 == c2) break;                                    /*見た目上文字列に変化がないとき*/
      if(*(str + i) == c1)                                   /*(str + i)の文字がc1の文字と同じとき*/
	{
	  *(str + i) = c2;                                   /*(str + i)の文字をc2の文字に置き換える*/
	  c++;                                               /*置き換えた文字を数える*/
	}
    }
  return c;                                                  /*置き換えた文字数を戻り値とする．*/
}

int split(char *str, char *ret[], char sep, int max)
{
  int i;                                                     /*forループ用*/
  int c = 0;                                                 /*ポインタの配列の指定用*/

  ret[c++] = str;                                            /*ret[0]にstrの先頭アドレスを代入*/

  for(i = 0; *(str + i) != '\0'&& c < max; i++)              /*cがmaxより小さいかつ入力文字列の終端に辿り着いていないときループ*/
    {
      if(*(str + i) == sep)                                  /*(str + i)がsepのとき*/
	{
	  *(str + i) = '\0';                                 /*(str + i)にNULLを代入*/
	  ret[c++] = str + (i + 1);                          /*ret[c]にNULL文字の"次の"アドレスを代入*/
	}
    }
  return c;                                                  /*文字列をいくつに分割したかを戻り値とする*/
}

int get_line(FILE *F, char *line)
{

  if(fgets(line, MAX_LINE, F) == NULL) return 0;     /*入力文字列が空のとき，0を戻り値とする．入力文字列は1024文字*/
  if(*line == ESC) cmd_quit("r");

  subst(line, '\n', '\0');                                   /*subst関数により，入力の改行文字を終端文字に置き換える*/
  return 1;                                                  /*入力文字列が存在したとき，1を戻り値とする*/
}

void parse_line(char *line)
{
  static int i = 1;                                          /*登録中止数カウント用*/
  char *ret[2] = {NULL, NULL};                               /*コマンド文字列のポインタ，引数文字列のポインタ用*/

  if(*line == '%')                                           /*入力文字列の1文字目が%のとき*/
    {
      line++;
      split(line, ret, ' ', 2);
      exec_command(ret[0], ret[1]);
    }
  else if(profile_data_nitems < 10000)                       /*入力がコマンドではなく，登録数が1万件以下のとき*/
    {
      new_profile(&profile_data_store[profile_data_nitems++] ,line);
    }
  else
    {
      fprintf(stderr, "Warning: 10000件を超える名簿データは読み込めません．計%d件の登録が中止されました．\n", i);
      i++;
    }
}

void exec_command(char *cmd, char *param)
{
  /*strcmpの導入*/

  switch (*cmd) {
  case 'Q': cmd_quit(param);   break;
  case 'C': cmd_check();  break;
  case 'P': cmd_print(param);  break;
  case 'R': cmd_read(param);   break;
  case 'W': cmd_write(param);  break;
  case 'F': cmd_find(param);   break;
  case 'S': cmd_sort(param);   break;
  default: fprintf(stderr, "不明なコマンド\"%s\"です．処理を中止しました．\n\n", cmd); break;/*エラーメッセージを表示*/
  }
}

void cmd_quit(char *param)
{
  char c = 65;

  if(param != NULL)
    if(*param == 'r') exit(0);
 
  while(1)
    {
      printf("終了しますか?(y/n)\n");                      /*確認メッセージ*/
      scanf("%c", &c);
      if(c != '\n')getchar(); /*getcharでの入力時に改行文字が残ってしまうため*/
      if(c == 'y')
  	{
  	  printf("正常終了．\n\n");
	  exit(0);
	}
      else if(c == 'n')
	{
	  printf("処理を中止しました．\n\n");
	  break;
	}
    }
}

void cmd_check(void)
{
  printf("%d profile(s)\n", profile_data_nitems);
}

void cmd_print(char *param)
{
  int a = 0;
  int i = 0;                                             /*forループ用*/

  /*atoi関数で正常に文字列をint値に変換できるかの確認を実施*/
  if(param == NULL) a = 0;
  if(param != NULL)
    {
      if(int_value_check(param))
	{
	  fprintf(stderr,"引数は数値である必要があります．処理を中止しました．\n\n");
	  return;
	}
      a = atoi(param);                                   /*文字列をint型の値に変換*/
    }

  /*aの絶対値がprofile_data_nitemsより大きいときかa=0のとき*/
  if(abs(a) >= profile_data_nitems|| a == 0) a = profile_data_nitems;

  if(a > 0)                                              /*引数が正の整数のとき及び例外*/
    {
      for(i = 0; i < a; i++)
        {
	  printf("Id    : %d\n", profile_data_store[i].id);
	  printf("Name  : %s\n", profile_data_store[i].name);
	  printf("Birth : %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
	  printf("Addr. : %s\n",profile_data_store[i].address);
	  printf("Comm. : %s\n\n",profile_data_store[i].biko);
	}
    }
  else if(a < 0)                                         /*引数が負の整数のとき*/
    {
      for(i = profile_data_nitems + a; i < profile_data_nitems; i++)
	{
	  printf("Id    : %d\n", profile_data_store[i].id);
	  printf("Name  : %s\n", profile_data_store[i].name);
	  printf("Birth : %04d-%02d-%02d\n", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
	  printf("Addr. : %s\n",profile_data_store[i].address);
	  printf("Comm. : %s\n\n",profile_data_store[i].biko);
	}
    }
}

void cmd_read(char *param)
{
  char LINE[MAX_LINE] = {0};
  FILE *fp;

  if(param == NULL)
    {
      fprintf(stderr, "実行には引数が必要です．処理を中止しました．\n\n");
      return;
    }

  if((fp = fopen(param, "r")) == NULL)                    /*指定されたファイル名が存在しない場合*/
    {
      fprintf(stderr, "\"%s\"を読み込めません．カレントディレクトリにファイルが存在しないか，読み取り許可がない可能性があります．\n\n", param);
      return;
    }

  while(get_line(fp, LINE))                               /*文字配列LINEに文字列を入力する*/
    {
      parse_line(LINE);                                   /*入力文字列がある場合，構文解析を行う*/
    }

  fclose(fp);
}

void cmd_write(char *param)
{
  int i;                                                  /*forループ用*/
  FILE *fp;

  if(param == NULL)
    {
      fprintf(stderr, "実行には引数が必要です．処理を中止しました．\n\n");
      return;
    }

  if((fp = fopen(param, "w")) == NULL)                    /*指定されたファイル名が存在しない場合*/
    {
      fprintf(stderr, "\"%s\"に書き込めません．書き込み許可がない可能性があります．\n\n", param);
      return;
    }

  /*CSV形式で出力*/
  for(i = 0; i < profile_data_nitems; i++)
    {
      fprintf(fp, "%d,", profile_data_store[i].id);
      fprintf(fp, "%s,", profile_data_store[i].name);
      fprintf(fp, "%d-%d-%d,", profile_data_store[i].birthday.y, profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
      fprintf(fp, "%s,",profile_data_store[i].address);
      fprintf(fp, "%s\n",profile_data_store[i].biko);
    }

  fclose(fp);
}
void cmd_find(char *param)
{
  int i = 0;                                              /*forループ用*/
  char num1[12];                                          /*int値を文字列に変換する際に使用*/
  char num2[36];                                          /*int値を文字列に変換する際に使用*/
  struct profile *p;

  if(param == NULL)
    {
      fprintf(stderr, "実行には引数が必要です．処理を中止しました．\n\n");
      return;
    }

  for(i = 0; i < profile_data_nitems; i++)
    {
      p = &profile_data_store[i];

      /*int値を文字列に変換して代入*/
      sprintf(num1,"%d", p->id);
      sprintf(num2,"%04d-%02d-%02d",(p->birthday).y, (p->birthday).m, (p->birthday).d);

      if(strcmp(param, num1) == 0 ||                      /*ID比較*/
	 strcmp(param, p->name) == 0 ||                   /*name比較*/
	 strcmp(param, num2) == 0 ||                      /*birthday比較*/
	 strcmp(param, p->address) == 0 ||                /*address比較*/
	 strcmp(param, p->biko) == 0)                     /*biko比較*/
	{                                                 /*該当名簿情報表示*/
	  printf("Id    : %d\n", p->id);
	  printf("Name  : %s\n", p->name);
	  printf("Birth : %04d-%02d-%02d\n", (p->birthday).y, (p->birthday).m, (p->birthday).d);
	  printf("Addr. : %s\n",p->address);
	  printf("Comm. : %s\n\n",p->biko);
	 }
    }
}

void cmd_sort(char *param)
{
  int a = 0;
  int a_buff = 0;                                          /*param変換後の値保持用*/  
  int i1 = 0;                                              /*バブルソート用の右端を決める*/
  int i2 = 0;                                              /*バブルソートの探索用*/
  char num1[36];                                           /*int値を文字列に変換する際に使用*/
  char num2[36];                                           /*int値を文字列に変換する際に使用*/
  struct profile *sp1;
  struct profile *sp2;
  char *cp1;                                               /*任意文字列の先頭アドレスを保持*/
  char *cp2;                                               /*任意文字列の先頭アドレスを保持*/

  /*atoi関数で正常に文字列をint値に変換できるかの確認を実施*/
  if(param == NULL)
    {
      fprintf(stderr, "実行には引数が必要です．処理を中止しました．\n\n");
      return;
    }
  if(int_value_check(param))
    {
      fprintf(stderr, "引数は数値である必要があります．処理を中止しました．\n\n");
      return;
    }
  a_buff = atoi(param);

  if(a_buff < 1 || a_buff > 5)
    {
      fprintf(stderr, "引数は1から5のいずれかの数値である必要があります．処理を中止しました．\n\n");
      return;
    }

  for(i1 = 0; i1 < profile_data_nitems; i1++)
    {
      for(i2 = 0; i2 < profile_data_nitems - i1 - 1; i2++)
	{
	  a = a_buff;                                      /*例外対策処理用で毎度aを初期化*/
	  sp1 = &profile_data_store[i2];                   /*名簿データの先頭アドレスを登録*/
	  sp2 = &profile_data_store[i2 + 1];               /*1つ後ろの名簿データの先頭アドレスを登録*/

	  if(a == 2)
	    {
	      cp1 = sp1->name;
	      cp2 = sp2->name;
	      if(strcmp(cp1, cp2) == 0) a = 1;             /*項目が一致していた場合は，IDの昇順になるように並び変える*/
	    }
	  
	  if(a == 3)
	    {
	      sprintf(num1, "%010d%010d%010d", (sp1->birthday).y, (sp1->birthday).m, (sp1->birthday).d);
	      sprintf(num2, "%010d%010d%010d", (sp2->birthday).y, (sp2->birthday).m, (sp2->birthday).d);
	      cp1 = num1;
	      cp2 = num2;
	      if(strcmp(cp1, cp2) == 0) a = 1;             /*項目が一致していた場合は，IDの昇順になるように並び変える*/
	    }
	  
	  if(a == 4)
	    {
	      cp1 = sp1->address;
	      cp2 = sp2->address;
	      if(strcmp(cp1, cp2) == 0) a = 1;             /*項目が一致していた場合は，IDの昇順になるように並び変える*/
	    }
	  if(a == 5)
	    {
	      cp1 = sp1->biko;
	      cp2 = sp2->biko;
	      if(strcmp(cp1, cp2) == 0) a = 1;             /*項目が一致していた場合は，IDの昇順になるように並び変える*/
	    }
 	  
	  if(a == 1)
	    {
	      sprintf(num1, "%010d", sp1->id);
	      sprintf(num2, "%010d", sp2->id);
	      cp1 = num1;
	      cp2 = num2;
	    }

	  if(strcmp(cp1, cp2) > 0) data_move(sp1, sp2);
	}
    }
}

void data_move(struct profile *sp1, struct profile *sp2)
{
  struct profile swap_data;
  
  /*sp1のデータをswap_dataに退避*/
  swap_data.id = sp1->id;
  strcpy(swap_data.name, sp1->name);
  swap_data.birthday.y = (sp1->birthday).y;
  swap_data.birthday.m = (sp1->birthday).m;
  swap_data.birthday.d = (sp1->birthday).d;
  strcpy(swap_data.address, sp1->address);
  swap_data.biko = sp1->biko;

  /*sp2のデータをsp1に移動*/
  sp1->id = sp2->id;
  strcpy(sp1->name, sp2->name);
  (sp1->birthday).y = (sp2->birthday).y;
  (sp1->birthday).m = (sp2->birthday).m;
  (sp1->birthday).d = (sp2->birthday).d;
  strcpy(sp1->address, sp2->address);
  sp1->biko = sp2->biko;
  
  /*swap_dataをsp2に移動*/
  sp2->id = swap_data.id;
  strcpy(sp2->name, swap_data.name);
  (sp2->birthday).y = (swap_data.birthday).y;
  (sp2->birthday).m = (swap_data.birthday).m;
  (sp2->birthday).d = (swap_data.birthday).d;
  strcpy(sp2->address, swap_data.address);
  sp2->biko = swap_data.biko;
}

void new_profile(struct profile *profile_p, char *line)
{
  char *ret[10];
  char *ret2[4];                               /*誕生日の情報を分割し，その先頭アドレスを保存*/
  char sep;                                    /*入力文字列の要素を区切るのに使用*/
  char sep2 = '-';                             /*誕生日の入力文字列にあるハイフンで区切るため，ハイフン*/
  int max = 5;
  int max2 = 4;
  int birth_c = 0;
  int MAX_BIKO = 0;                            /*備考の文字数カウント用*/
  static int i = 0;                            /*入力項目の行番号監視*/
  
  i++;
  sep = input_format_check(line);
  if(sep == 0)
    {
      fprintf(stderr, "情報はID，名前，誕生日，住所，備考の順で入力される必要があり，カンマ区切り，セミコロン区切り，タブ区切りのいずれかの体裁である必要があります．処理を中止しました(項目番号:%d)．\n\n", i);
      profile_data_nitems--;                   /*処理中止により，構造体に情報を書き込まないため*/
      return;
    }
  split(line, ret, sep, max);                  /*ID，名前などの情報を分割する*/

  /*atoi関数で正常に文字列をint値に変換できるかの確認を実施*/
  if(int_value_check(ret[0]))
    {
      fprintf(stderr,"IDの項目は数値である必要があります．処理を中止しました(項目番号:%d)．\n\n", i);
      profile_data_nitems--;
      return;
    }

  birth_c = split(ret[2], ret2, sep2, max2);   /*誕生日の年，月，日を分割する*/
  if(birth_c != 3)                             /*誕生日の年，月，日を正常に分割できない場合*/
    {
      fprintf(stderr, "誕生日は\"年-月-日\"の形で入力される必要があります．処理を中止しました(項目番号:%d)．\n\n", i); /*年，月，日に分割できない場合，処理を停止*/
      profile_data_nitems--;                   /*処理中止により，構造体に情報を書き込まないため*/
      return;
    }

  /*atoi関数で正常に文字列をint値に変換できるかの確認を実施*/
  if(int_value_check(ret2[0]) ||
     int_value_check(ret2[1]) ||
     int_value_check(ret2[2]) )
    {
      fprintf(stderr,"誕生年，月，日の項目は数値である必要があります．処理を中止しました(項目番号:%d)．\n\n", i);
      profile_data_nitems--;
      return;
    }

  /*構造体への情報の書き込み処理*/
  profile_p->id = atoi(ret[0]);                /*IDの書き込み*/
  strncpy(profile_p->name, ret[1], 69);        /*名前の書き込み*/
  (profile_p->birthday).y = atoi(ret2[0]);     /*誕生年の書き込み*/
  (profile_p->birthday).m = atoi(ret2[1]);     /*誕生月の書き込み*/
  (profile_p->birthday).d = atoi(ret2[2]);     /*誕生日の書き込み*/
  strncpy(profile_p->address, ret[3], 69);     /*住所の書き込み*/

  MAX_BIKO = strlen(ret[4]) + 1;               /*備考情報の文字数のカウント*/

  profile_p->biko = (char *)malloc(sizeof(char)* MAX_BIKO); /*文字数分だけメモリ確保*/
  strncpy(profile_p->biko, ret[4], MAX_BIKO);  /*備考の書き込み*/
}

int int_value_check(char *str)
{
  if((*str >= 48 && *str <= 57) || *str == 43 || *str == 45) str++;
  else return 1;

  while(*str)                             /*入力文字列の終端に辿り着くまでループ*/
    {
      if(*str >= 48 && *str <= 57) str++; /*確認する文字が0~9の場合，次の文字を確認*/
      else return 1;                      /*確認する文字が0~9で無い場合，戻り値1*/
    }
  return 0;  
}

int input_format_check(char *str)
{
  int com_c = 0;                         /*コンマの使用回数のカウント*/
  int semi_c = 0;                        /*セミコロンの使用回数のカウント*/
  int tab_c = 0;                         /*タブの使用回数のカウント*/

  while(*str)                            /*入力文字列の終端に辿り着くまでループ*/
    {
      if(*str == ',')com_c++;            /*カンマ区切り*/
      if(*str == ';')semi_c++;           /*セミコロン区切り*/
      if(*str == 9)tab_c++;              /*タブ区切り*/
      str++;
    }
    
  if(com_c == 4)return 44;
  if(semi_c == 4)return 59;
  if(tab_c == 4)return 9;

  return 0;                              /*例外*/
}

int main(void)
{
  char LINE[MAX_LINE] = {0};              /*標準入力文字列(1行分)はmain関数で管理*/

  while(get_line(stdin, LINE))            /*文字配列LINEに文字列を入力する*/
    {
      parse_line(LINE);                   /*入力文字列がある場合，構文解析を行う*/
    }
  return 0;
}
