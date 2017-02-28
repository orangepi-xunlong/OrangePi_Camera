#include <stdio.h>
#include <stdlib.h>
#include <OrangePiV4L2/OrangePiV4L2.h>

int main(int argc, char *argv[])
{
    struct OrangePi_v4l2_device *dev;

    dev = OrangePi_device_init();
    OrangePi_device_close(dev);

    printf("1 + 1 = %d\n", add(1, 1));
    printf("2 + 1 = %d\n", sub(2, 1));
    printf("Hello World\n");  
    return 0;    
}
