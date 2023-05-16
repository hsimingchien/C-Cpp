#include <stdio.h>
#include <stdarg.h>

typedef unsigned int XOS_STATUS;
typedef unsigned short WORD16;
typedef unsigned char BYTE;
#define vsnprintf(b,c,f,a) vsnprintf(b,c,f,a)
#define PRINT_RCP_COM     (WORD16)(0)


#define     LOG_CRITICAL     2
#define     LOG_ERROR        3
#define     LOG_NOTICE       5
#define     LOG_DEBUG        7

XOS_STATUS g_rcpPrintFlag = 1;

void set_rcp_print(void)
{
    g_rcpPrintFlag = 1;
}
void clear_rcp_print(void)
{
    g_rcpPrintFlag = 0;
}

/* 打印宏 */
#define RCP_PRINTF(moudle, level, str)\
{\
    if(g_rcpPrintFlag==1)\
    {\
        XOS_SysLog("%s File: %s Line %d Func %s\t %s", level, __FILE__,__LINE__,__FUNCTION__,str);\
    }\
}

#define RCP_PRINT_DEBUG(str)    RCP_PRINTF(PRINT_RCP_COM, "[\033[1;34mLOG_DEBUG\033[0m]", str)
#define RCP_PRINT_NORMAL(str)   RCP_PRINTF(PRINT_RCP_COM, "[\033[0;34mLOG_NOTICE\033[0m]", str)
#define RCP_PRINT_WARNING(str)  RCP_PRINTF(PRINT_RCP_COM, "[\033[1;31mLOG_ERROR\033[0m]", str)
#define RCP_PRINT_ERROR(str)    RCP_PRINTF(PRINT_RCP_COM, "[\033[0;31mLOG_CRITICAL\033[0m]", str)

XOS_STATUS XOS_SysLog(char* pcFmt, ...)
{
    static char buff[8092];
    va_list args;

    //此函数后续实现为日志文件
    va_start(args, pcFmt);
    vsnprintf( buff, sizeof(buff), pcFmt, args); 
    va_end(args);
    
    buff[sizeof(buff)-1] = '\0';
    printf("%s\n",buff);
    return 0;
}

void main()
{
    RCP_PRINT_DEBUG("Hello World.1");
    RCP_PRINT_NORMAL("Hello World.2");
    RCP_PRINT_WARNING("Hello World.3");
    RCP_PRINT_ERROR("Hello World.4");
    return;
}

