#include <stdio.h>
#include <stdlib.h>
#include <OrangePiV4L2/OrangePiV4L2.h>

int main(int argc, char *argv[])
{
    struct OrangePi_v4l2_device *dev;

    dev = OrangePi_device_init();
    OrangePi_device_capture(dev);
    OrangePi_device_close(dev);

    DEBUG_ORANGEPI("OrangePi");
    printf("Hello World\n");  
    return 0;    
}
