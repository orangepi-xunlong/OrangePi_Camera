#ifndef _ORANGEPIV4L2_H_
#define _ORANGEPIV4L2_H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/videodev2.h>

struct buffer
{
    unsigned long int *start;
    size_t length;
};

struct OrangePi_BMP_HEADER
{
    unsigned short bfType;
    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;  
};

struct OrangePi_BMP_INFO
{
    unsigned long biSize;
    unsigned long biWidth;
    unsigned long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long biCompression;
    
    unsigned long biSizeImage;
    unsigned long biXPelsPerMeter;
    unsigned long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;    
};

struct OrangePi_buffer {
    int n_buffers;
    FILE *Out_fd;
    FILE *YUV_fd;
    FILE *JPG_fd;
    struct buffer *Raw_buffers;
    char *YUV_buffer;
    size_t current_length;
    unsigned char *newBuf;
};

struct OrangePi_v4l2_device;

struct OrangePi_v4l2_driver {
    int  (*open)(struct OrangePi_v4l2_device *dev);
    void (*close)(struct OrangePi_v4l2_device *dev);
    int  (*init)(struct OrangePi_v4l2_device *dev);
    int  (*capable)(struct OrangePi_v4l2_device *dev);
    int  (*check_format)(struct OrangePi_v4l2_device *dev);
    int  (*capture)(struct OrangePi_v4l2_device *dev);
    void (*current_framer)(struct OrangePi_v4l2_device *dev);
};

struct OrangePi_v4l2_device {
    int  fd;
    FILE *buffer_fd;
    char *device_name;
    int  width;
    int  height;
    int  format;
    unsigned int timeout;
    unsigned int fps;
    struct OrangePi_buffer *buffers;
    void *private_data;
    struct OrangePi_v4l2_driver *drv;
};

struct OrangePi_v4l2_configure {
    char *Platform;
    char *Camera_name;
    char *Device_name;
    int Capture_width;
    int Capture_Height;
    int Capture_Format;
    int Capture_FPS;  
    int Buffer_Number;
    int Capture_Timeout;
    int DEBUG;
};

/* OrangePi Device */
extern struct OrangePi_v4l2_driver OrangePi;

/* Initialize OrangePi Camera device */
int OrangePi_V4L2_init(struct OrangePi_v4l2_device *);

/* Close a OrangePi Camera device */
void OrangePi_V4L2_exit(struct OrangePi_v4l2_device *);

/* Capture a picture */
void OrangePi_device_capture(struct OrangePi_v4l2_device *);

/* Cove YUYV 2 JPEG */
void OrangePi_Process_Image(struct OrangePi_v4l2_device *, const char *);

/* Capture one picture */
void OrangePi_Capture_One(struct OrangePi_v4l2_device *, const char *);

/* Get BMP picture */
void OrangePi_BMP(struct OrangePi_v4l2_device *, const char *);

int write_JPEG_file(const char *, unsigned char*, int, int, int);

void OrangePi_JPEG(struct OrangePi_v4l2_device *, const char *);

#endif
