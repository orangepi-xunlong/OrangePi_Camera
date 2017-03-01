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

struct OrangePi_buffer {
    int n_buffers;
    FILE *Out_fd;
    FILE *YUV_fd;
    FILE *JPG_fd;
    struct buffer *Raw_buffers;
    char *YUV_buffer;
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
    int  (*open)(struct OrangePi_v4l2_device *dev);
    void (*close)(struct OrangePi_v4l2_device *dev);
    int  (*init)(struct OrangePi_v4l2_device *dev);
    int  (*capable)(struct OrangePi_v4l2_device *dev);
    int  (*check_format)(struct OrangePi_v4l2_device *dev);
    int  (*capture)(struct OrangePi_v4l2_device *dev);
    void (*current_framer)(struct OrangePi_v4l2_device *dev);
};

struct OrangePi_v4l2_configure {
    char *Camera_name;
    char *Device_name;
    int Capture_width;
    int Capture_Height;
    int Capture_Format;
    int Capture_FPS;  
    int Buffer_Number;
    int Capture_Timeout;
};

/*
 * Debug system
 */
#define DEBUG_INFO 1

#if (DEBUG_INFO == 1)
#define DEBUG_ORANGEPI printf
#else
#define DEBUG_ORANGEPI printf
#endif

/* OrangePi Device */
extern struct OrangePi_v4l2_device OrangePi;

/* Initialize OrangePi Camera device */
struct OrangePi_v4l2_device *OrangePi_device_init(void);

/* Close a OrangePi Camera device */
void OrangePi_device_close(struct OrangePi_v4l2_device *);

/* Capture a picture */
void OrangePi_device_capture(struct OrangePi_v4l2_device *);

/* Cove YUYV 2 JPEG */
void OrangePi_Process_Image(struct OrangePi_v4l2_device *, const char *);

/* Capture one picture */
void OrangePi_device_captureOne(struct OrangePi_v4l2_device *, const char *);

extern int write_JPEG_file(const char *, unsigned char*, int, int, int);

#endif
