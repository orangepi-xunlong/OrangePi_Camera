#include <stdio.h>
#include <stdlib.h>
#include <OrangePiV4L2/OrangePiV4L2.h>
#include <OrangePiV4L2/OrangePi_Config.h>

int main(int argc, char *argv[])
{
    struct OrangePi_v4l2_device *dev;

    dev = (struct OrangePi_v4l2_device *)malloc(
            sizeof(struct OrangePi_v4l2_device));
    if (!dev) {
        printf("[OrangePi Camera] Can't allow memory!\n");
        return -1;    
    }

    OrangePi_V4L2_init(dev);
    if (strcmp(OrangePi_Get_Platform(), "OrangePi_RDA") == 0)
        /* Output BMP */
        OrangePi_BMP(dev, "./Before.bmp");
    else
        /* Output JPEG */
        OrangePi_Capture_One(dev, "./Before.jpg");
    OrangePi_V4L2_exit(dev);

    /* free resource */
    free(dev);

    printf("Hello World\n");  
    return 0;    
}
