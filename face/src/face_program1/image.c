//*********************************************
//作成日：2015/10/19-25
//改訂日：
//プログラムについて：
//*********************************************
#include<stdio.h>
#include<string.h>
#include<math.h>
#include"image.h"

//filenameのBitmapファイルを読み込み、高さと幅、RGB情報をimg構造体に入れる
Image *Read_Bmp(char *filename)
{
  int i,j;
  int real_width;              //データ上の1行分のバイト数
  unsigned int width, height;  //画像の横と縦のピクセル数
  unsigned int color;          //何bitのBitmapファイルであるか
  FILE *fp;
  unsigned char header_buf[HEADERSIZE];  //ヘッダ情報を取り込む
  unsigned char *bmp_line_data;  //画像データ1行分
  Image *img;

  if((fp = fopen(filename, "rb")) == NULL){
    fprintf(stderr, "Error: %s could not read.", filename);
    return NULL;
  }

  fread(header_buf, sizeof(unsigned char), HEADERSIZE, fp);
//ヘッダ部分すべてを取り込む

//最初の2バイトがBM(Bitmapファイルの印)であるか
  if(strncmp(header_buf, "BM", 2)){
    fprintf(stderr, "Error: %s is not Bitmap file.", filename);
    return NULL;
  }

  memcpy(&width, header_buf+18, sizeof(width));  //画像の見た目上の幅を取得
  memcpy(&height, header_buf+22, sizeof(height));  //画像の高さを取得
  memcpy(&color, header_buf+28, sizeof(unsigned int));//何bitのBMPであるかを取得

  //24bitでなければ終了
  if(color != 24){
    fprintf(stderr, "Error: %s is not 24bit color image", filename);
    return NULL;
  }

  //RGB情報は画像の1行分が4byteの倍数でなければならないためそれに合わせている
  real_width = width*3 + width%4;

  //画像の1行分のRGB情報を取ってくるためのバッファを動的に取得
  if((bmp_line_data = (unsigned char *)malloc(sizeof(unsigned char)*real_width)) == NULL){
    fprintf(stderr, "Error: Allocation error./n");
    return NULL;
  }

  //RGB情報を取り込むためのバッファを動的に取得
  if((img = Create_Image(width, height)) == NULL){
    free(bmp_line_data);
    fclose(fp);
    return NULL;
  }

  //BitmapファイルのRGB情報は左下から右へ、下から上に並んでいる
  for(i=0; i<height; i++){
    fread(bmp_line_data, 1, real_width, fp);
    for(j=0; j<width; j++){
      img->data[(height-i-1)*width + j].b = bmp_line_data[j*3];
      img->data[(height-i-1)*width + j].g = bmp_line_data[j*3 + 1];
      img->data[(height-i-1)*width + j].r = bmp_line_data[j*3 + 2];
    }
  }

  fclose(fp);
  return img;
}

int Write_Bmp(char *filename, Image *img)
{
  int i,j;
  FILE *fp;
  int real_width;
  unsigned char *bmp_line_data;  //画像一行分のRGB情報を格納する
  unsigned char header_buf[HEADERSIZE];  //ヘッダを格納する
  unsigned int file_size;
  unsigned int offset_to_data;
  unsigned long info_header_size;
  unsigned int planes;
  unsigned int color;
  unsigned long compress;
  unsigned long data_size;
  long xppm;
  long yppm;

  if((fp = fopen(filename, "wb")) == NULL){
    fprintf(stderr, "Error: %s could not open.", filename);
    return 1;
  }

  real_width = img->width*3 + img->width%4;

  //ここからヘッダ作成
  file_size = img->height *real_width + HEADERSIZE;
  offset_to_data = HEADERSIZE;
  info_header_size = INFOHEADERSIZE;
  planes = 1;
  color = 24;
  compress = 0;
  data_size = img->height * real_width;
  xppm = 1;
  yppm = 1;

  header_buf[0] = 'B';
  header_buf[1] = 'M';
  memcpy(header_buf + 2, &file_size, sizeof(file_size));
  header_buf[6] = 0;
  header_buf[7] = 0;

  header_buf[8] = 0;
  header_buf[9] = 0;
  memcpy(header_buf + 10, &offset_to_data, sizeof(file_size));
  header_buf[11] = 0;
  header_buf[12] = 0;
  header_buf[13] = 0;

  memcpy(header_buf + 14, &info_header_size, sizeof(info_header_size));
  header_buf[15] = 0;
  header_buf[16] = 0;
  header_buf[17] = 0;
 
  memcpy(header_buf + 18, &img->width, sizeof(img->width));
  memcpy(header_buf + 22, &img->height, sizeof(img->height));
  memcpy(header_buf + 26, &planes, sizeof(planes));
  memcpy(header_buf + 28, &color, sizeof(color));
  memcpy(header_buf + 30, &compress, sizeof(compress));
  memcpy(header_buf + 34, &data_size, sizeof(data_size));
  memcpy(header_buf + 38, &xppm, sizeof(xppm));
  memcpy(header_buf + 42, &yppm, sizeof(yppm));
  header_buf[46] = 0;
  header_buf[47] = 0;
  header_buf[48] = 0;
  header_buf[49] = 0;
  header_buf[50] = 0;
  header_buf[51] = 0;
  header_buf[52] = 0;
  header_buf[53] = 0;

  //ヘッダの書き込み
  fwrite(header_buf, sizeof(unsigned char), HEADERSIZE, fp);

  if((bmp_line_data = (unsigned char *)malloc(sizeof(unsigned char)*real_width)) == NULL){
    fprintf(stderr, "Error: Allocation error.\n");
    fclose(fp);
    return 1;
  }

  //RGBj情報の書き込み
  for(i=0; i<img->height; i++){
    for(j=0; j<img->width; j++){
      bmp_line_data[j*3]=img->data[(img->height-i-1)*img->width+j].b;
      bmp_line_data[j*3 + 1]=img->data[(img->height-i-1)*img->width+j].g;
      bmp_line_data[j*3 + 2]=img->data[(img->height-i-1)*img->width+j].r;
    }
    //RGB情報を4バイトの倍数に合わせている
    for(j=img->width*3; j<real_width; j++){
      bmp_line_data[j] = 0;
    }
    fwrite(bmp_line_data, sizeof(unsigned char), real_width, fp);
  }
  free(bmp_line_data);
  fclose(fp);
  return 0;
}

Image *Create_Image(int width, int height)
{
  Image *img;
  if((img = (Image *)malloc(sizeof(Image))) == NULL){
    fprintf(stderr, "Allocation error\n");
    return NULL;
  }
  if((img->data = (Rgb *)malloc(sizeof(Rgb)*width*height)) == NULL){
    printf(stderr, "Allocation error\n");
    free(img);
  }
  img->width = width;
  img->height = height;

  return img;
}

//動的に取得したRGB情報の開放
void Free_Image(Image *img)
{
  free(img->data);
  free(img);
}

//*********************************************
//作成日：2015/10/25
//改訂日：
//プログラムについて：実験に追加するプログラム
//*********************************************

//bmpのImageを受け取ってグレースケールに変換する
Image *Change_Gray_Scale(Image *img)
{
  int i,j;
  unsigned char r,g,b;
  unsigned int tmp;  //ループ内で配列の番号を省略するため
  unsigned char y;  //グレースケールの値
  unsigned int height = img->height;
  unsigned int width = img->width;

  //imgのRGBを1pixelずつ読み取ってグレースケールに変換
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      tmp = (height-i-1)*width+j;  //現在の走査位置
      b = img->data[tmp].b;
      g = img->data[tmp].g;
      r = img->data[tmp].r;

      //グレースケールの計算式
      y = 0.299*r + 0.587*g + 0.114*b;

      Tame_Rgb(&(img->data[tmp]), y);  //RGB要素すべてに同じ値を入れる
    }


  }

  return img;
}

//bmpのImageを受け取って2値画像に変換する
//成功すればポインタを返す
Image *Change_Binary_Scale(Image *img)
{
  int i,j;
  unsigned char r,g,b;
  unsigned int tmp;  //ループ内で配列の番号を省略するため
  unsigned char y;  //2値
  unsigned int height = img->height;
  unsigned int width = img->width;

  //imgのRGBを1pixelずつ読み取って2値に変換
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      tmp = (height-i-1)*width+j;  //現在の走査位置
      b = img->data[tmp].b;
      g = img->data[tmp].g;
      r = img->data[tmp].r;

      //2値画像に変換
      y = (r+g+b) / 3;
      y = (y > 128) ? 255 : 0;

      Tame_Rgb(&(img->data[tmp]), y);  //RGB要素すべてに同じ値を入れる
    }
  }

  return img;
}

//座標(x1,y1)から座標(x2.y2)へ線を引く
//成功すれば0,失敗すれば1を返す
int Draw_Line(Image *img, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  int height,width;
  int x,y;  //ループ用
  int lx, ly, sx, sy;  //largeとsmall成分（囲まれた範囲の走査のため）
  height = img->height;
  width = img->width;
  if(x1>x2){
    lx = x1;
    sx = x2;
  }
  else{
    lx = x2;
    sx = x1;
  }
  if(y1>y2){
    ly = y1;
    sy = y2;
  }
  else{
    ly = y2;
    sy = y1;
  }

  //xまたはyの最大値がwidth, heightを超えていないかチェック
  if(lx>width || ly>height){
    fprintf(stderr, "Error:座標の位置が不正です.\n");
    return 1;
  }

  int tmp;  //走査位置
  if(x2 != x1){  //分母が0でないとき
    float a = (float)(y2-y1)/(x2-x1);  //2点間の直選の傾き
    float b = y1 - a*x1;  //2点間の直線の切片
    int t;  //xのパラメータ(小数点以下切り捨て)
    for(y=sy; y<ly; y++){  //yに囲まれた範囲
	for(x=sx; x<lx; x++){  //xに囲まれた範囲
	  t = a*x + b;
	  if(y == t){
	    tmp = y*width+x;  //現在の走査位置
	    img->data[tmp].r = 0;
	    img->data[tmp].b = 200;
	    img->data[tmp].g = 0;
	  }
	}
    }
  }
  else{  //x1=x2のとき
    for(y=sy; y<ly; y++){  //yに囲まれた範囲
	for(x=sx; x<lx; x++){  //xに囲まれた範囲
	  tmp = y*width+x;  //現在の走査位置
	  img->data[tmp].r = 0;
	  img->data[tmp].b = 200;
	  img->data[tmp].g = 0;
	}
    }
  }
  return 0;
}

//RGB要素をならす(すべてyの値にする)関数
void Tame_Rgb(Rgb *data, unsigned char y)
{
  data->r = y;
  data->g = y;
  data->b = y;
}
