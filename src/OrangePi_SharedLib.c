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

int add(int a, int b)
{
    return a + b;    
}
