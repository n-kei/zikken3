//*********************************************
//作成日：2015/10/25
//改訂日：
//プログラムについて：実験に追加するプログラム
//*********************************************
#include<stdio.h>
#include"addon.h"

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
