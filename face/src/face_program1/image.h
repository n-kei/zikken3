#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#define FILEHEADERSIZE 14
#define INFOHEADERSIZE 40
#define HEADERSIZE (FILEHEADERSIZE + INFOHEADERSIZE)

typedef struct{
  unsigned char b;
  unsigned char g;
  unsigned char r;
}Rgb;

typedef struct{
  unsigned int height;
  unsigned int width;
  Rgb *data;
}Image;

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

//取得に成功すればポインタを失敗すればNULLを返す
Image *Read_Bmp(char *filename);

//書き込みに成功ずれば0を失敗すれば1を返す
int Write_Bmp(char *filename, Image *img);

//Imageを作成し、RGB情報もwidth*heigh分だけ動的に取得する
//成功すればポインタを、失敗すればNULLを返す
Image *Create_Image(int width, int height);
//Imageを開放する
void Free_Image(Image *img);

//bmpのImageを受け取ってグレースケールに変換する
Image *Change_Gray_Scale(Image *img);

//bmpのImageを受け取って2値画像に変換する
Image_Binary *Change_Binary_Scale(Image *img);

//座標(x1,y1)から座標(x2.y2)へ線を引く
//成功すれば0、失敗すれば1を返す
int Draw_Line(Image *img, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

//RGB要素すべてに同じ値yを入れる関数
void Tame_Rgb(Rgb *data, unsigned char y);

#endif
