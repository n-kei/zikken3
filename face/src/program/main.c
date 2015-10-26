//*********************************************
//作成日：2015/10/19
//改訂日：
//プログラムについて：bitmapファイルをimage構造体に読み込み、
//image構造体をbitmap形式で出力する
//*********************************************
#include"bitmap.h"
#include"addon.h"
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char *argv[])  //(読み込みファイル名, 出力ファイル名)
{
  if(argc != 3){
    fprintf(stderr, "Usage:program <inputfile> <outputfile>\n");
    exit(1);
  }
  Image *colorimg;

  if((colorimg = Read_Bmp(argv[1])) == NULL){
    exit(1);
  }

  //ここに顔認証のアルゴリズムを記述する
  unsigned char selection;
  fprintf(stdout, "---Select Mode---\n 1:Gray \n 2:Binary \n 3:Facial recognition \n >");

  scanf("%c",&selection);
  //  while((selection = getc(stdin)) != '1'||'2'||'3');

  if('1' == selection)
    Change_Gray_Scale(colorimg);
  else if('2' == selection)
    Change_Binary_Scale(colorimg);
  else if('3' == selection)
    ;
  //ここまで

  //線のテスト用
  int test;
  printf("testmode:1\n");
  scanf("%d",&test);
  if(test == 1){
    if(0 != Draw_Line(colorimg, 0, 0, 100, 100))
      exit;
  }
  //--------------------------------

  if(Write_Bmp(argv[2], colorimg)){
    exit(1);
  }
  Free_Image(colorimg);

  return 0;
}
