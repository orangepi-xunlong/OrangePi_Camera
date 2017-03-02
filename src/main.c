#include <stdio.h>
#include <stdlib.h>
#include <OrangePiV4L2/OrangePiV4L2.h>
#include <OrangePiV4L2/OrangePi_Config.h>

int main(int argc, char *argv[])
{
    struct OrangePi_v4l2_device *dev;

    OrangePi_Parse_Configure();
    OrangePi_Show_Current_Camera_Configure();

    dev = OrangePi_device_init();
//    OrangePi_device_captureOne(dev, "./Before.jpg");
    OrangePi_BMP(dev, "./Before.bmp");
    OrangePi_device_close(dev);

    OrangePi_Configure_Release();
    printf("Hello World\n");  
    return 0;    
}
