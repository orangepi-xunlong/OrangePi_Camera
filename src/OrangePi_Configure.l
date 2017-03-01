%option noyywrap

%{
#include <OrangePiV4L2/OrangePi_Config.h>
#include <OrangePiV4L2/OrangePi_Debug.h>
#include <OrangePiV4L2/OrangePiV4L2.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/videodev2.h>

#define CONFIG_PATH "/etc/OrangePi_Camera.conf"
int chars = 0;
int words = 0;
int lines = 0;

int totchars = 0;
int totwords = 0;
int totlines = 0;

int Calc_Table[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

struct OrangePi_v4l2_configure OrangePi_Camera_Config;

static void OrangePi_Set_Camera_Name(const char *);
static void OrangePi_Set_Device_Name(const char *);
static void OrangePi_Set_Capture_width(const char *);
static void OrangePi_Set_Capture_Height(char *);
static void OrangePi_Set_Capture_Format(char *);
static void OrangePi_Set_Capture_FPS(char *);
static void OrangePi_Set_Buffer_Number(char *);
static void OrangePi_Set_Capture_Timeout(char *);
static void OrangePi_Set_Platform(const char *);
static void OrangePi_Set_DEBUG(const char *);

%}

%%
[ \t]*#.*\n            { /* Comment */ }
Platform.*\n           { OrangePi_Set_Platform(yytext); }
Camera_Name.*\n        { OrangePi_Set_Camera_Name(yytext); }
Device_Name.*\n        { OrangePi_Set_Device_Name(yytext); }
Capture_Format.*\n     { OrangePi_Set_Capture_Format(yytext); }
Capture_FPS.*\n        { OrangePi_Set_Capture_FPS(yytext); }
Capture_Width.*\n      { OrangePi_Set_Capture_width(yytext); }
Capture_Height.*\n     { OrangePi_Set_Capture_Height(yytext); }
Capture_Timeout.*\n    { OrangePi_Set_Capture_Timeout(yytext); }
Buffer_Number.*\n      { OrangePi_Set_Buffer_Number(yytext); }
DEBUG.*\n              { OrangePi_Set_DEBUG(yytext); }
.			           { /* Ignore other */ }

%%

static void str2int(const char *str, int *value, int n)
{
    int i;

    *value = 0;
    
    for(i = 0; i < n; i++) 
        *value += (str[i] - '0') * Calc_Table[n - i - 1];  
}

/*
 * Set Platform
 */
static void OrangePi_Set_Platform(const char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    OrangePi_Camera_Config.Platform = (char *)malloc(len);
    strncpy(OrangePi_Camera_Config.Platform, tmp, len);
    OrangePi_Camera_Config.Platform[len - 1] = '\0';
}

/*
 * Set Camera Name
 */
static void OrangePi_Set_Camera_Name(const char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    OrangePi_Camera_Config.Camera_name = (char *)malloc(len);
    strncpy(OrangePi_Camera_Config.Camera_name, tmp, len);
    OrangePi_Camera_Config.Camera_name[len - 1] = '\0';
}

/*
 * Set Device name
 */
static void OrangePi_Set_Device_Name(const char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    OrangePi_Camera_Config.Device_name = (char *)malloc(len);
    strncpy(OrangePi_Camera_Config.Device_name, tmp, len);
    OrangePi_Camera_Config.Device_name[len - 1] = '\0';
}

/*
 * Set Capture width
 */
static void OrangePi_Set_Capture_width(const char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    str2int(tmp, &OrangePi_Camera_Config.Capture_width, len - 1);
}

/*
 * Set Capture Height
 */
static void OrangePi_Set_Capture_Height(char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    str2int(tmp, &OrangePi_Camera_Config.Capture_Height, len - 1);
}

/*
 * Set Buffer Number
 */
static void OrangePi_Set_Buffer_Number(char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    str2int(tmp, &OrangePi_Camera_Config.Buffer_Number, len - 1);
}

/*
 * Set Capture Timeout
 */
static void OrangePi_Set_Capture_Timeout(char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    str2int(tmp, &OrangePi_Camera_Config.Capture_Timeout, len - 1);
}

/*
 * Set Capture Format
 */
static void OrangePi_Set_Capture_Format(char *name)
{
    char *tmp,*tmp1;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    tmp1 = (char *)malloc(len);
    strncpy(tmp1, tmp, len);
    tmp1[len - 1] = '\0';
    /* Search Table */
    if (strcmp(tmp1, "V4L2_PIX_FMT_YVU410") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YVU410;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YVU420") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YVU420;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUYV") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUYV;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YYUV") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YYUV;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YVYU") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YVYU;
    if (strcmp(tmp1, "V4L2_PIX_FMT_UYVY") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_UYVY;
    if (strcmp(tmp1, "V4L2_PIX_FMT_VYUY") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_VYUY;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV422P") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV422P;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV411P") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV411P;
    if (strcmp(tmp1, "V4L2_PIX_FMT_Y41P") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_Y41P;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV444") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV444;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV555") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV555;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV565") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV565;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV32") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV32;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV410") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV410;
    if (strcmp(tmp1, "V4L2_PIX_FMT_YUV420") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_YUV420;
    if (strcmp(tmp1, "V4L2_PIX_FMT_HI240") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_HI240;
    if (strcmp(tmp1, "V4L2_PIX_FMT_HM12") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_HM12;
    if (strcmp(tmp1, "V4L2_PIX_FMT_M420") == 0)
        OrangePi_Camera_Config.Capture_Format = V4L2_PIX_FMT_M420;
}

/*
 * Set Capture FPS
 */
static void OrangePi_Set_Capture_FPS(char *name)
{
    char *tmp;
    int len;

    tmp = strstr(name, "=") + 1;
    len = strlen(tmp);
    str2int(tmp, &OrangePi_Camera_Config.Capture_FPS, len - 1);
}

/*
 * Set Debug level
 */
static void OrangePi_Set_DEBUG(const char *name)
{
    char *tmp;

    tmp = strstr(name, "=") + 1;
    if (strncmp(tmp, "ON", 2) == 0)
        OrangePi_Camera_Config.DEBUG = 1;
    else
        OrangePi_Camera_Config.DEBUG = 0;
}

/*
 * Export: Get Platform
 */
char *OrangePi_Get_Platform(void)
{
    return OrangePi_Camera_Config.Platform;    
}

/*
 * Export: Get Camera Name
 */
char *OrangePi_Get_Camera_Name(void)
{
    return OrangePi_Camera_Config.Camera_name;    
}

/*
 * Export: Get Device Name
 */
char *OrangePi_Get_Device_Name(void)
{
    return OrangePi_Camera_Config.Device_name;    
}

/*
 * Export: Get Capture Width
 */
int OrangePi_Get_Capture_Width(void)
{
    return OrangePi_Camera_Config.Capture_width;    
}

/*
 * Export: Get Capture Height
 */
int OrangePi_Get_Capture_Height(void)
{
    return OrangePi_Camera_Config.Capture_Height;    
}

/*
 * Export: Get Capture Format
 */
int OrangePi_Get_Capture_Format(void)
{
    return OrangePi_Camera_Config.Capture_Format;    
}

/*
 * Export: Get Camera Name
 */
int OrangePi_Get_Capture_FPS(void)
{
    return OrangePi_Camera_Config.Capture_FPS;    
}

/*
 * Export: Get Capture Timeout
 */
int OrangePi_Get_Capture_Timeout(void)
{
    return OrangePi_Camera_Config.Capture_Timeout;    
}

/*
 * Export: Get Buffer Number
 */
int OrangePi_Get_Buffer_Number(void)
{
    return OrangePi_Camera_Config.Buffer_Number;    
}

/*
 * Export: Get Debug level
 */
int OrangePi_Get_DEBUG(void)
{
    return OrangePi_Camera_Config.DEBUG;    
}

/*
 * Display all configure
 */
void OrangePi_Show_Current_Camera_Configure(void)
{
    DEBUG_ORANGEPI("===== Camera Configure =====\n");
    DEBUG_ORANGEPI("Platform:\t\t%s\n", OrangePi_Get_Platform());    
    DEBUG_ORANGEPI("Camera Name:\t\t%s\n", OrangePi_Get_Camera_Name());    
    DEBUG_ORANGEPI("Device Name:\t\t%s\n", OrangePi_Get_Device_Name());    
    DEBUG_ORANGEPI("Capture Width:\t\t%d\n", OrangePi_Get_Capture_Width());    
    DEBUG_ORANGEPI("Capture Height:\t\t%d\n", OrangePi_Get_Capture_Height());    
    DEBUG_ORANGEPI("Capture Format:\t\t%d\n", OrangePi_Get_Capture_Format());    
    DEBUG_ORANGEPI("Capture FPS:\t\t%d\n", OrangePi_Get_Capture_FPS());    
    DEBUG_ORANGEPI("Capture Timeout:\t%d\n", OrangePi_Get_Capture_Timeout());    
    DEBUG_ORANGEPI("Buffer Number:\t\t%d\n", OrangePi_Get_Buffer_Number());    
    DEBUG_ORANGEPI("============================\n");    
}

/*
 * Release all resource 
 */
void OrangePi_Configure_Release(void)
{
    if (OrangePi_Camera_Config.Platform != NULL)
        free(OrangePi_Camera_Config.Platform);    
    if (OrangePi_Camera_Config.Camera_name != NULL)
        free(OrangePi_Camera_Config.Camera_name);    
    if (OrangePi_Camera_Config.Device_name != NULL)
        free(OrangePi_Camera_Config.Device_name);    
}

/*
 * Parsing Configure file from "/etc/OrangePi_Camera.conf"
 */
int OrangePi_Parse_Configure(void)
{

	FILE *f = fopen(CONFIG_PATH, "r");

	if (!f) {
		perror(CONFIG_PATH);
		return -1;
	}
		
    yyrestart(f);
	yylex();
	fclose(f);

	return 0;
}

