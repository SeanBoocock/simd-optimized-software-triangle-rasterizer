
#ifndef LOGGING_H_
#define LOGGING_H_

/*	Utilities for logging events for debugging purposes
 *	Taken from: http://bwta.googlecode.com/svn›trunk›src›util.cpp which maintained under LGPL
*/	

#include <stdarg.h>
#include <stdio.h>
#include <cstring>


inline void log(char* time, const char* text, ...)
{
	FILE * pFile;
	fopen_s (&pFile,"renderer.log","a");
	char logBuffer[1024];

	va_list ap;
	va_start(ap, text);
	vsprintf_s(logBuffer,1024,text,ap );
	va_end(ap);
	if (pFile!=NULL)
	{
		fputs ( time, pFile);
		fputs (logBuffer,pFile);
		fputs ("\n",pFile);
		fclose (pFile);
	}
}

#endif