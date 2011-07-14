//Timer.h

#include <windows.h>

class Timer{
private:
	LARGE_INTEGER frequency;
    LARGE_INTEGER start;

public:
	Timer()
	{ 
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start); 
	}

	void reset () 
	{ 
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start); 
	}

	double getMicroseconds()
	{ 
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart;
	}

	double getMilliseconds()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)frequency.QuadPart;
	}

	double getSeconds()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return (double)(end.QuadPart - start.QuadPart)  / (double)frequency.QuadPart;
	}
};