#ifndef _ORANGEPI_CONFIG_H_
#define _ORANGEPI_CONFIG_H_

/* Get OrangePi configure */
int OrangePi_Parse_Configure(void);

/* Show current camera configure */
void OrangePi_Show_Current_Camera_Configure(void);

char *OrangePi_Get_Camera_Name(void);
char *OrangePi_Get_Device_Name(void);
int OrangePi_Get_Capture_Width(void);
int OrangePi_Get_Capture_Height(void);
int OrangePi_Get_Capture_Format(void);
int OrangePi_Get_Capture_FPS(void);
int OrangePi_Get_Buffer_Number(void);
int OrangePi_Get_DEBUG(void);
int OrangePi_Get_Capture_Timeout(void);
char *OrangePi_Get_Platform(void);

#endif
