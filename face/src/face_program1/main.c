//*********************************************
//作成日：2015/10/19
//改訂日：
//プログラムについて：bitmapファイルをimage構造体に読み込み、
//image構造体をbitmap形式で出力する
//*********************************************
#include"image.h"
#include<stdlib.h>
#include<stdio.h>

#define ORIGIN_X 200
#define END_X 200
#define LENGTH_Y 550
//#define MODE_TEST
#define MODE_RUN

#ifdef MODE_TEST
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
#endif

#ifdef MODE_RUN

int main(int argc, char *argv[])  //(読み込みファイル名, 出力ファイル名)
{
  char *inputImageName;
  char *outputImageName;
  Image *colorImage;
  Image_Binary *binaryData;
  unsigned int i, j, tmp;
  unsigned int head_x = 0;
  unsigned int head_y = 0;
  
  if(argc != 3){
    fprintf(stderr, "Usage:program <inputfile> <outputfile>\n");
    exit(1);
  }

  inputImageName = argv[1];
  outputImageName = argv[2];
  
  if((colorImage = Read_Bmp(inputImageName)) == NULL){
    exit(1);
  }
  
  binaryData = Change_Binary_Scale(colorImage);
  
  for(i = 0; i < colorImage->height; i++) {
    for(j = 0; j < colorImage->width; j++) {
      tmp = (colorImage->height-i-1)*colorImage->width+j;  //現在の走査位置
      if(binaryData->binary[tmp] == 0) {
	head_y = i;
	head_x = j;
	break;
      }
    }
    if(head_x && head_y)
      break;
  }
  Draw_Line(colorImage, head_x - ORIGIN_X, head_y, head_x + END_X, head_y);
  Draw_Line(colorImage, head_x - END_X, head_y + LENGTH_Y, head_x + END_X, head_y + LENGTH_Y);
  Draw_Line(colorImage, head_x + END_X, head_y, head_x + END_X, head_y + LENGTH_Y);
  Draw_Line(colorImage, head_x - ORIGIN_X, head_y, head_x - ORIGIN_X, head_y + LENGTH_Y);
  
  Write_Bmp(outputImageName, colorImage);
  Free_Image(colorImage);

  return(0);
}

#endif
