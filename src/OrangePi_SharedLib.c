#include <OrangePiV4L2/OrangePiV4L2.h>
#include <OrangePiV4L2/YUYV2BMP.h>
#include <OrangePiV4L2/OrangePi_Config.h>

/*
 * Capture a picture
 */
void OrangePi_device_capture(struct OrangePi_v4l2_device *dev)
{
    dev->drv->capture(dev);    
}

/*
 * Cover YUYV to JPEG
 */
void OrangePi_Process_Image(struct OrangePi_v4l2_device *dev, const char *JPEG_PATH)
{
    write_JPEG_file(JPEG_PATH, dev->buffers->YUV_buffer, 100, dev->width, dev->height);
}

/*
 * Capture one Picture
 */
void OrangePi_Capture_One(struct OrangePi_v4l2_device *dev, const char *JPEG_PATH)
{
    OrangePi_Show_Current_Camera_Configure();
    dev->drv->capture(dev);
    write_JPEG_file(JPEG_PATH, dev->buffers->YUV_buffer, 100, dev->width, dev->height);
}

/*
 * Cover YUYV to JPEG
 */
void OrangePi_JPEG(struct OrangePi_v4l2_device *dev, const char *JPEG_PATH)
{
    imageProcess(dev->buffers->YUV_buffer, (char *)JPEG_PATH, dev->width, dev->height, 50);    
}

/*
 * Cover YUYV to BMP
 */
void OrangePi_BMP(struct OrangePi_v4l2_device *dev, const char *BMP_PATH)
{
    if (strcmp(OrangePi_Get_Platform(), "OrangePi_RDA") == 0) {
        dev->buffers->newBuf = calloc(dev->buffers->current_length * 3 / 2, sizeof(unsigned char));
        if (!dev->buffers->newBuf) {
            printf("cannot assign the memory!\n");
            return;    
        }
        OrangePi_YUYV2RGB(dev);     
        OrangePi_Move_Noise(dev);
        OrangePi_Store_BMP(dev, BMP_PATH);
        
    } else {
        FILE *fd;

        fd = fopen(BMP_PATH, "wb");

        Get_BMP(dev->buffers->YUV_buffer, dev->width, dev->height, fd);
    
        fclose(fd);
    }
}

/*
 * Initialize a v4l2 device
 */
int OrangePi_V4L2_init(struct OrangePi_v4l2_device *dev)
{  
    struct OrangePi_v4l2_configure *conf;
    
    conf = (struct OrangePi_v4l2_configure *)malloc(
                sizeof(struct OrangePi_v4l2_configure));
    if (!conf) {
        printf("[OrangePi Camera] Can't allow memory to Configure!\n");
        return -1;    
    }
    OrangePi_Parse_Configure(conf); 
    dev->drv = &OrangePi;
    dev->drv->init(dev);

    return 0;
}

/*
 * Release a v4l2 device
 */
void OrangePi_V4L2_exit(struct OrangePi_v4l2_device *dev)
{
    dev->drv->close(dev);
    dev->drv = NULL;
    free(dev->buffers->newBuf);
    OrangePi_Configure_Release();
    free(OrangePi_Get_Private_Configure());
}

