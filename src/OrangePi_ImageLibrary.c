#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <jpeglib.h>
#include <OrangePiV4L2/YUYV2BMP.h>
#include <OrangePiV4L2/OrangePiV4L2.h>

#define YUYV_TYPEA  1
#define BMP_TYPEA   0
#define BMP_TYPEB   1

static RGB16 rgb16;
static RGB24 rgb24;
static BMP   bmp24;

/* Initialize the buffer */
void Init_Buffer(unsigned int w, unsigned int h)
{
    rgb16.len = w * h * 2;
    rgb24.len = w * h * 3;
    bmp24.len = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + rgb24.len;
	
    rgb16.buf = (unsigned char *)malloc(rgb16.len);
	
    rgb24.buf = (unsigned char *)malloc(rgb24.len);
	
    bmp24.buf = (unsigned char *)malloc(bmp24.len);
}

/* Free buffere */
void Free_Buffer()
{
    if(rgb16.buf)
        free(rgb16.buf);
		
    if(rgb24.buf)
        free(rgb24.buf);
		
    if(bmp24.buf)
        free(bmp24.buf);
		
    rgb16.len= 0;
    rgb24.len= 0;
    bmp24.len= 0;
	
    rgb16.buf = NULL;
    rgb24.buf = NULL;
    bmp24.buf = NULL;
}

/*
 * YUYV to RGB16
 */
void YUYV_2_RGB16(unsigned char *yuv, unsigned int w, unsigned int h)
{
    unsigned char y0,cb0,y1,cr0;
    int r, g, b;
	
    int len   = w * h * 2;
    unsigned short  *prgb = (unsigned short *)rgb16.buf;

    len /= 4;
    while(len--) {
        y0  = *yuv++;
        cb0 = *yuv++;
        y1  = *yuv++;
        cr0 = *yuv++;
	
        r = YCbCrtoR(y0, cb0, cr0);
        g = YCbCrtoG(y0, cb0, cr0);
        b = YCbCrtoB(y0, cb0, cr0);
	
        if (r > 255) r = 255;
        if (r < 0  ) r = 0;
        if (g > 255) g = 255;
        if (g < 0  ) g = 0;
        if (b > 255) b = 255;
        if (b < 0  ) b = 0;
		
        // 5:6:5 16bit format
        *prgb++ = (((unsigned short )r>>3)<<11) | (((unsigned short )g>>2)<<5) | 
			    (((unsigned short )b>>3)<<0);
	
        r = YCbCrtoR(y1, cb0, cr0);
        g = YCbCrtoG(y1, cb0, cr0);
        b = YCbCrtoB(y1, cb0, cr0);

        if (r > 255) r = 255;
        if (r < 0  ) r = 0;
        if (g > 255) g = 255;
        if (g < 0  ) g = 0;
        if (b > 255) b = 255;
        if (b < 0  ) b = 0;
		
        // 5:6:5 16bit format
        *prgb++ = (((unsigned short )r>>3)<<11) | (((unsigned short )g>>2)<<5) | 
			    (((unsigned short )b>>3)<<0);
	}
}

/*
 * YUYV to RGB24
 */
void YUYV_2_RGB24(unsigned char *yuv, unsigned int w, unsigned int h)
{
    unsigned char y0,cb0,y1,cr0;
    int r, g, b;
	
    int len  = w * h * 2;
    unsigned char *prgb = rgb24.buf;

    len /= 4;
    while(len--) {
        y0  = *yuv++;
        cb0 = *yuv++;
        y1  = *yuv++;
        cr0 = *yuv++;
	
        r = YCbCrtoR(y0, cb0, cr0);
        g = YCbCrtoG(y0, cb0, cr0);
        b = YCbCrtoB(y0, cb0, cr0);
	
        if (r > 255) r = 255;
        if (r < 0  ) r = 0;
        if (g > 255) g = 255;
        if (g < 0  ) g = 0;
        if (b > 255) b = 255;
        if (b < 0  ) b = 0;

#if YUYV_TYPEA
	*prgb++ = ((r * 220 / 256) & 0xFF);
	*prgb++ = ((g * 220 / 256) & 0xFF);
	*prgb++ = ((b * 220 / 256) & 0xFF);
#else
        *prgb++ = r;
        *prgb++ = g;
        *prgb++ = b;
#endif
        r = YCbCrtoR(y1, cb0, cr0);
        g = YCbCrtoG(y1, cb0, cr0);
        b = YCbCrtoB(y1, cb0, cr0);

        if (r > 255) r = 255;
        if (r < 0  ) r = 0;
        if (g > 255) g = 255;
        if (g < 0  ) g = 0;
        if (b > 255) b = 255;
        if (b < 0  ) b = 0;

#if YUYV_TYPEA
	*prgb++ = ((r * 220 / 256) & 0xFF);
	*prgb++ = ((g * 220 / 256) & 0xFF);
	*prgb++ = ((b * 220 / 256) & 0xFF);
#else	
        *prgb++ = r;
	*prgb++ = g;
        *prgb++ = b;
#endif
    }
}

/*RGB24 to BMP*/
unsigned char *RGB_2_BMP(unsigned char *yuv, unsigned int w, unsigned int h)
{
    unsigned char *pbmp;
    unsigned char *prgb24;
    int i,j;
    BMPINFOHEADER bi;
    BMPFILEHEADER bf;
    /*yuv4:2:2 ---> rgb24*/
    YUYV_2_RGB24(yuv, w, h);

    /* Init head */
    bi.biSize   = 0x28;
    bi.biWidth  = w;
    bi.biHeight = h;
    bi.biPlanes = 1;
    bi.biBitCount   = 24;
    bi.biCompression= 0;
    bi.biSizeImage  = w * h * 3;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed      = 0;
    bi.biClrImportant = 0;

    bf.bfType = 0x4D42;	
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;
    bf.bfOffBits   = 54;    
        
    pbmp  = bmp24.buf;
    memcpy(pbmp, &bf, 14);
    pbmp += 14;
    memcpy(pbmp, &bi, 40);
    pbmp += 40;
	
    prgb24= rgb24.buf;
#if BMP_TYPEA
    i = rgb24.len;
    memcpy(pbmp,prgb24,i);
#elif BMP_TYPEB
    i = w * 3;
    prgb24 += ((h - 2) * i);
    for(j = 0 ; j < h ; j++) {
	memcpy(pbmp,prgb24,i);
	pbmp   += i;
	prgb24 -= i;
    }
#else
    prgb24 += (rgb24.len-1);
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            *pbmp++ = *prgb24--;
            *pbmp++ = *prgb24--;
            *pbmp++ = *prgb24--;
        }
    }
#endif  
 
    return bmp24.buf;
}

int write_JPEG_file(const char *filename, unsigned char*yuvData, 
			int quality,int image_width,int image_height)
{

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;    
    JSAMPROW row_pointer[1];  
    int row_stride;    
    JSAMPIMAGE  buffer;
    unsigned char *pSrc,*pDst;
    int max_line = cinfo.max_v_samp_factor*DCTSIZE; 
    int counter;
    int src_width[3],src_height[3];
    unsigned char *rawData[3];
    int band,i,buf_width[3],buf_height[3];

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return -1;
    }

    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = image_width;  
    cinfo.image_height = image_height;
    cinfo.input_components = 3;   
    cinfo.in_color_space = JCS_RGB;  

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE );
    
    cinfo.raw_data_in = TRUE;
    cinfo.jpeg_color_space = JCS_YCbCr;
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
    
    jpeg_start_compress(&cinfo, TRUE);
 
    buffer = (JSAMPIMAGE) (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo,
                                 JPOOL_IMAGE, 3 * sizeof(JSAMPARRAY)); 

    for(band=0; band <3; band++) {
        buf_width[band] = cinfo.comp_info[band].width_in_blocks * DCTSIZE;
        buf_height[band] = cinfo.comp_info[band].v_samp_factor * DCTSIZE;
        buffer[band] = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo,
                                 JPOOL_IMAGE, buf_width[band], buf_height[band]);
    } 

    rawData[0]=yuvData;
    rawData[1]=yuvData+image_width*image_height;
    rawData[2]=yuvData+image_width*image_height*5/4;

    for(i=0;i<3;i++) {
        src_width[i]=(i==0)?image_width:image_width/2;
        src_height[i]=(i==0)?image_height:image_height/2;
    }

    for(counter=0; cinfo.next_scanline < cinfo.image_height; counter++) {   
        for(band=0; band <3; band++) {
             int mem_size = src_width[band];

             pDst = (unsigned char *) buffer[band][0];
             pSrc = (unsigned char *) rawData[band] + counter*buf_height[band] * src_width[band];

             for(i=0; i <buf_height[band]; i++) {
                  memcpy(pDst, pSrc, mem_size);
                  pSrc += src_width[band];
                  pDst += buf_width[band];
             }
        }
        jpeg_write_raw_data(&cinfo, buffer, max_line);

    }
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    return 0;
}


static void jpegWrite(unsigned char* img,char *jpegFilename,int width,int height,int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    FILE *outfile = fopen( jpegFilename, "wb" );
  
    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);
 
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
   
    jpeg_set_defaults(&cinfo);
  
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);
 
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &img[cinfo.next_scanline * cinfo.image_width *
        cinfo.input_components];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    
    jpeg_finish_compress(&cinfo);
    
    jpeg_destroy_compress(&cinfo);

    fclose(outfile);
}

static void YUV422toRGB888(int width, int height, unsigned char *src, unsigned char *dst)
{
    int line, column;
    unsigned char *py, *pu, *pv;
    unsigned char *tmp = dst;

    py = src;
    pu = src + 1;
    pv = src + 3;
    #define CLIP(x) ( (x)>=0xFF ? 0xFF : ( (x) <= 0x00 ? 0x00 : (x) ) )
    for (line = 0; line < height; ++line) {
        for (column = 0; column < width; ++column) {
            *tmp++ = CLIP((double)*py + 1.402*((double)*pv-128.0));
            *tmp++ = CLIP((double)*py - 0.344*((double)*pu-128.0) -0.714*((double)*pv-128.0));
            *tmp++ = CLIP((double)*py + 1.772*((double)*pu-128.0));
            
            py += 2;
            
            if ((column & 1)==1) {
                pu += 4;
                pv += 4;
            }
        }
    }
}

void imageProcess(const void* yuv,char *FilePath,int width,int height,int quality)
{
    unsigned char *src = (unsigned char*)yuv;
    unsigned char *dst = malloc(width * height * 3 * sizeof(char));

    YUV422toRGB888(width,height,src,dst);
   
    jpegWrite(dst,FilePath,width,height,quality);
	
    free(dst);
}

void Get_BMP(unsigned char *yuv,unsigned int width,unsigned int height,FILE *fd)
{
    /* Initialize the all buffer */
    Init_Buffer(width,height);
    /* RGB to BMP */
    RGB_2_BMP(yuv,width,height);
    /* Write data to file */
    fwrite(bmp24.buf,bmp24.len,1,fd);
    /* Free buffer */
    Free_Buffer();
}
