#include <OrangePiV4L2/OrangePiV4L2.h>

/*
 * Initialize a OrangePi Camera device
 */
struct OrangePi_v4l2_device *OrangePi_device_init(void)
{
    /* Prepare v4l2 device */
    OrangePi.init(&OrangePi);
    
    return &OrangePi;    
}

/*
 * Close a OrangePi Camera device
 */
void OrangePi_device_close(struct OrangePi_v4l2_device *dev)
{
    dev->close(dev);    
}

/*
 * Capture a picture
 */
void OrangePi_device_capture(struct OrangePi_v4l2_device *dev)
{
    dev->capture(dev);    
}

/*
 * Cover YUYV to JPEG
 */
void OrangePi_Process_Image(struct OrangePi_v4l2_device *dev, const char *JPEG_PATH)
{
    write_JPEG_file(JPEG_PATH, dev->buffers->YUV_buffer, 100, dev->width, dev->height);
}

/*
 * Cover YUYV to BMP
 */
void OrangePi_BMP(struct OrangePi_v4l2_device *dev, const char *BMP_PATH)
{
    FILE *fd = fopen(BMP_PATH, "wb");

    Get_BMP(dev->buffers->YUV_buffer, dev->width, dev->height, fd);
    
    fclose(fd);    
}

/* 
 * Capture one picture
 */
void OrangePi_device_captureOne(struct OrangePi_v4l2_device *dev, const char *JPEG_PATH)
{
    dev->capture(dev);
    OrangePi_Process_Image(dev, JPEG_PATH);        
}
