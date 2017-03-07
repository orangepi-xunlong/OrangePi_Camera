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

void OrangePi_YUYV2RGB(struct OrangePi_v4l2_device *dev)  
{  
    unsigned char YUYV[4],RGB[6];  
    int j,k,i;     
    unsigned int location=0; 
    unsigned char *starter = (unsigned char *)dev->buffers->YUV_buffer;
 
    j=0;  
    for(i=0;i < dev->buffers->current_length;i+=4) {  
        YUYV[0]=starter[i];//Y0  
        YUYV[1]=starter[i+1];//U  
        YUYV[2]=starter[i+2];//Y1  
        YUYV[3]=starter[i+3];//V  
        if(YUYV[0]<1) {  
            RGB[0]=0;  
            RGB[1]=0;  
            RGB[2]=0;  
        } else {  
            RGB[0]=YUYV[0]+1.772*(YUYV[1]-128);//b  
            RGB[1]=YUYV[0]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g  
            RGB[2]=YUYV[0]+1.402*(YUYV[3]-128);//r  
        }  

        if(YUYV[2]<0) {  
            RGB[3]=0;  
            RGB[4]=0;  
            RGB[5]=0;  
        } else {  
            RGB[3]=YUYV[2]+1.772*(YUYV[1]-128);//b  
            RGB[4]=YUYV[2]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[3]-128);//g  
            RGB[5]=YUYV[2]+1.402*(YUYV[3]-128);//r  
        }  
      
        for(k=0;k<6;k++) {  
            if(RGB[k]<0)  
                RGB[k]=0;  
            if(RGB[k]>255)  
                RGB[k]=255;  
        }  
   
        if( j % (dev->width * 3)==0) {  
            location = (dev->height - j / (dev->width * 3)) * (dev->width * 3);  
        }  
        bcopy(RGB, dev->buffers->newBuf + location + ( j % (dev->width *3)), sizeof(RGB));  
        j += 6;         
    }  
    return;  
} 

void OrangePi_Move_Noise(struct OrangePi_v4l2_device *dev) 
{  
    int i,j,k,temp[3],temp1[3];  
    unsigned char BGR[13*3];  
    unsigned int sq,sq1,loc,loc1;  
    int h = dev->height, w = dev->width;
    unsigned int a;
    unsigned char *newBuf = dev->buffers->newBuf;
  
    for(i=2;i<h-2;i++) {  
        for(j=2;j<w-2;j++) {  
            memcpy(BGR,newBuf+(i-1)*w*3+3*(j-1),9);  
            memcpy(BGR+9,newBuf+i*w*3+3*(j-1),9);  
            memcpy(BGR+18,newBuf+(i+1)*w*3+3*(j-1),9);  
            memcpy(BGR+27,newBuf+(i-2)*w*3+3*j,3);  
            memcpy(BGR+30,newBuf+(i+2)*w*3+3*j,3);  
            memcpy(BGR+33,newBuf+i*w*3+3*(j-2),3);  
            memcpy(BGR+36,newBuf+i*w*3+3*(j+2),3);  
            memset(temp,0,4*3);  
                  
            for(k=0;k<9;k++) {  
                temp[0]+=BGR[k*3];  
                temp[1]+=BGR[k*3+1];  
                temp[2]+=BGR[k*3+2];  
            }  
            temp1[0]=temp[0];  
            temp1[1]=temp[1];  
            temp1[2]=temp[2];  
                
            for(k=9;k<13;k++) {  
                temp1[0]+=BGR[k*3];  
                temp1[1]+=BGR[k*3+1];  
                temp1[2]+=BGR[k*3+2];  
            }  
                
            for(k=0;k<3;k++) {  
                temp[k]/=9;  
                temp1[k]/=13;  
            }  
                
            sq=0xffffffff;loc=0;  
            sq1=0xffffffff;loc1=0;  
           
            for(k=0;k<9;k++) {  
                a=abs(temp[0]-BGR[k*3])+abs(temp[1]-BGR[k*3+1])+abs(temp[2]-BGR[k*3+2]);  
                if(a<sq) {  
                    sq=a;  
                    loc=k;  
                }  
            }  
                
            for(k=0;k<13;k++) {  
                a=abs(temp1[0]-BGR[k*3])+abs(temp1[1]-BGR[k*3+1])+abs(temp1[2]-BGR[k*3+2]);  
                if(a<sq1) {  
                    sq1=a;  
                    loc1=k;  
                }  
            }  
                  
            newBuf[i*w*3+3*j]=(unsigned char)((BGR[3*loc]+BGR[3*loc1])/2);  
            newBuf[i*w*3+3*j+1]=(unsigned char)((BGR[3*loc+1]+BGR[3*loc1+1])/2);  
            newBuf[i*w*3+3*j+2]=(unsigned char)((BGR[3*loc+2]+BGR[3*loc1+2])/2);
  
            /*还是有些许的噪点  
            temp[0]=(BGR[3*loc]+BGR[3*loc1])/2;  
            temp[1]=(BGR[3*loc+1]+BGR[3*loc1+1])/2;  
            temp[2]=(BGR[3*loc+2]+BGR[3*loc1+2])/2;  
            sq=abs(temp[0]-BGR[loc*3])+abs(temp[1]-BGR[loc*3+1])+abs(temp[2]-BGR[loc*3+2]);  
            sq1=abs(temp[0]-BGR[loc1*3])+abs(temp[1]-BGR[loc1*3+1])+abs(temp[2]-BGR[loc1*3+2]);  
            if(sq1<sq) loc=loc1;  
            newBuf[i*w*3+3*j]=BGR[3*loc];  
            newBuf[i*w*3+3*j+1]=BGR[3*loc+1];  
            newBuf[i*w*3+3*j+2]=BGR[3*loc+2];*/  
        }  
    }  
    return;  
}  

void OrangePi_YUYV2RGB1(struct OrangePi_v4l2_device *dev)  
{  
    unsigned char YUYV[3],RGB[3];    
    int j,k,i;     
    unsigned int location=0;
    unsigned char *starter = (unsigned char *)dev->buffers->YUV_buffer;

    memset(YUYV, 0, 3);  
    j = 0;  
    for(i = 0; i < dev->buffers->current_length; i+=2) {  
        YUYV[0]=starter[i];//Y0  
        if(i%4==0)  
            YUYV[1]=starter[i+1];//U  
            //YUYV[2]=starter[i+2];//Y1  
        if(i%4==2)  
            YUYV[2]=starter[i+1];//V  
        if(YUYV[0]<1) {  
            RGB[0]=0;  
            RGB[1]=0;  
            RGB[2]=0;  
        } else {  
            RGB[0]=YUYV[0]+1.772*(YUYV[1]-128);//b  
            RGB[1]=YUYV[0]-0.34413*(YUYV[1]-128)-0.71414*(YUYV[2]-128);//g  
            RGB[2]=YUYV[0]+1.402*(YUYV[2]-128);//r  
        }  
      
        for(k=0;k<3;k++) {  
            if(RGB[k]<0)  
                RGB[k]=0;  
            if(RGB[k]>255)  
                RGB[k]=255;  
        }  
        
        if(j % (dev->width * 3) == 0) {  
            location=(dev->height - j / (dev->width * 3)) * (dev->width * 3);  
        }  
        bcopy(dev->buffers->newBuf, 
                dev->buffers->newBuf + location + (j % (dev->width * 3)),sizeof(RGB));  
        j+=3;         
    }  
    return;  
} 

void OrangePi_Store_YUYV(struct OrangePi_v4l2_device *dev, const char *file_name)  
{  
    FILE *fp = fopen(file_name, "wb");  
        
    if (fp < 0) {  
        printf("open frame data file failed\n");  
        return;  
    }  

    fwrite(dev->buffers->YUV_buffer, 1, dev->buffers->current_length, fp);  
    fclose(fp);  
    printf("Capture one frame saved in %s\n", file_name);  
        
    return;  
}

static void _Create_BMP_Header(struct OrangePi_v4l2_device *dev,
            struct OrangePi_BMP_HEADER *bfh, struct OrangePi_BMP_INFO *bih)
{
    bfh->bfType = (unsigned short)0x4D42;
    bfh->bfSize = (unsigned long)(14 + 40 + dev->width * dev->height * 3);
    bfh->bfReserved1 = 0;
    bfh->bfReserved2 = 0;
    bfh->bfOffBits = (unsigned long)(14 + 40);
                              
    bih->biBitCount = 24;
    bih->biWidth = dev->width;
    bih->biHeight = dev->height;
    bih->biSizeImage = dev->height * dev->width * 3;
    bih->biClrImportant = 0;
    bih->biClrUsed = 0;
    bih->biCompression = 0;
    bih->biPlanes = 1;
    bih->biSize = 40;
    bih->biXPelsPerMeter = 0x00000ec4;
    bih->biYPelsPerMeter = 0x00000ec4;    
}

void OrangePi_Store_BMP(struct OrangePi_v4l2_device *dev, const char *file_name)  
{  
    struct OrangePi_BMP_HEADER bfh;
    struct OrangePi_BMP_INFO bih;
    int n_len;
    FILE *fp1 = fopen(file_name, "wb"); 
     
    if (fp1 < 0) {  
        printf("open frame data file failed\n");  
        return;  
    }  
    _Create_BMP_Header(dev, &bfh, &bih);
    n_len = dev->buffers->current_length * 3 / 2;
    
    fwrite(&bfh,sizeof(bfh),1,fp1);  
    fwrite(&bih,sizeof(bih),1,fp1);  
    fwrite(dev->buffers->newBuf, 1, n_len, fp1);  
    fclose(fp1);  
    printf("Change one frame saved in %s\n", file_name);  
    return;  
}      
