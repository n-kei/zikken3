#ifndef __BITMAP_H_INCLUDED__
#define __BITMAP_H_INCLUDED__

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

//取得に成功すればポインタを失敗すればNULLを返す
Image *Read_Bmp(char *filename);

//書き込みに成功ずれば0を失敗すれば1を返す
int Write_Bmp(char *filename, Image *img);

//Imageを作成し、RGB情報もwidth*heigh分だけ動的に取得する
//成功すればポインタを、失敗すればNULLを返す
Image *Create_Image(int width, int height);
//Imageを開放する
void Free_Image(Image *img);

#endif /*__BITMAP_H_INCLUDED__*/
