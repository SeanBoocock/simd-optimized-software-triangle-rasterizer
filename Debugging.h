
#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <Windows.h>
#include "Logging.h"

static int trianglesStarted = 0;
static int pixelsPassedZTest = 0;
static int pixelsPassedLEETest = 0;
static int trianglesPassedClipTest = 0;

//#define DEBUG
#ifndef DEBUG
#define DEBUG_PRINT(x, ...)
#else
#define DEBUG_PRINT(x, ...)	do {	SYSTEMTIME st;				\
									char timeBuffer[17];		\
									GetSystemTime(&st);			\
									sprintf_s(timeBuffer,"%02d:%02d:%02d.%02d -  ", st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);	\
									printf(timeBuffer);			\
									printf(x,__VA_ARGS__);		\
									log(timeBuffer,x,__VA_ARGS__);			\
							} while(0)
#endif


#endif