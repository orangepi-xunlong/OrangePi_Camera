#ifndef YUVTOBMP_H
#define YUVTOBMP_H

/*
 * YUV4：2：2转RGB
 * y0 cb0 y1 cr0 ---> r0 g0 b0 r1 g1 b1 
 * r0 = YCbCrtoR(y0, cb0, cr0);
 * g0 = YCbCrtoG(y0, cb0, cr0);
 * b0 = YCbCrtoB(y0, cb0, cr0);
 * r1 = YCbCrtoR(y1, cb0, cr0);
 * g1 = YCbCrtoG(y1, cb0, cr0);
 * b1 = YCbCrtoB(y1, cb0, cr0);
 * YUYV转RGB公式
 */
#define YCbCrtoR(Y,Cb,Cr) (1000000*Y + 10370705*(Cr-128))/1000000
#define YCbCrtoG(Y,Cb,Cr) (1000000*Y - 337633*(Cb-128) - 698001*(Cr-128))/1000000
#define YCbCrtoB(Y,Cb,Cr) (1000000*Y + 1732446*(Cb-128))/1000000

/*rgb16缓存 len: w*h*2 */
typedef struct __rgb16data
{
	int len;	/*rgb16缓存长度*/
	unsigned char* buf;	/*rgb16缓存首地址*/
}RGB16;

/*rgb24缓存 len: w*h*3 */
typedef struct __rgb24data
{
	int len;	/*rgb24缓存长度*/
	unsigned char* buf;	/*rgb24缓存首地址*/
}RGB24;

typedef struct tagBMPFILEHEADER
{
	unsigned short  bfType;		// the flag of bmp, value is "BM"
	unsigned int bfSize;		// size BMP file ,unit is bytes
	unsigned int bfReserved1;	// 0
	unsigned int bfReserved2;	// 0
	unsigned int bfOffBits;		// must be 54
}BMPFILEHEADER;

/*BMP信息头*/
typedef struct tagBMPINFOHEADER
{
	unsigned int biSize;		// must be 0x28
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short  biPlanes;		// must be 1
	unsigned short  biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}BMPINFOHEADER;

#define WIDTHBYTES(i)  ((i+31)/32*4)

/*bmp缓存 len: bf+bi+rgb */
typedef struct __bmpdata
{
	int len;	/*54+rgb24.len*/
	unsigned char* buf;	/*bmp缓存首地址*/
}BMP;



void yuvtorgb16(unsigned char *yuv, unsigned int w, unsigned int h);
unsigned short * getrgb16();

void yuvtorgb24(unsigned char *yuv, unsigned int w, unsigned int h);
void* getrgb24();

unsigned char *rgbtobmp(unsigned char *yuv, unsigned int w, unsigned int h);
int getbmplen();

void init_conv(unsigned int w, unsigned int h);

void uninit_conv();

void create_bmp(FILE *fd);

#endif
