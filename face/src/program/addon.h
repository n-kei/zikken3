#ifndef ADDON_H
#define ADDON_H
#include"bitmap.h"

typedef struct{
  unsigned int height;
  unsigned int width;
  unsigned char *gray;  //グレースケールの濃さ
}Image_Gray;

typedef struct{
  unsigned int height;
  unsigned int width;
  unsigned char *binary;  //白黒の2値
}Image_Binary;

//bmpのImageを受け取ってグレースケールに変換する
Image *Change_Gray_Scale(Image *img);

//bmpのImageを受け取って2値画像に変換する
Image *Change_Binary_Scale(Image *img);

//座標(x1,y1)から座標(x2.y2)へ線を引く
//成功すれば0、失敗すれば1を返す
int Draw_Line(Image *img, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

//RGB要素すべてに同じ値yを入れる関数
void Tame_Rgb(Rgb *data, unsigned char y);

#endif /*ADDON_H*/
