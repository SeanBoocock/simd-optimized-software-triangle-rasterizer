
#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>
#include "RenderTarget.h"
#include "Constants.h"

#define IDI_TUTORIAL1 107

class Window
{
public:
	Window(	int incWidth = DEFAULT_WIDTH,
			int incHeight = DEFAULT_HEIGHT,
			char* incWindowName = "Window", 
			char* incClassName = "WindowClass"	)	:	hWnd(nullptr),
														hInstance(nullptr),
														windowName(incWindowName),
														windowClassName(incClassName),
														width(incWidth),
														height(incHeight),
														currentFrameBuffer(nullptr)
	{
		
	}
	
	~Window()
	{
		if(currentFrameBuffer)
			delete currentFrameBuffer;
	}

	HRESULT Initialize();

	HRESULT Present(RenderTarget* target);

	bool MessagePump();

protected:
	HRESULT CreateWinWindow();
	HRESULT RegisterWindow();

private:
	unsigned int width;
	unsigned int height;
	HWND hWnd;
	HDC deviceContext;
	HINSTANCE hInstance;
	const char* windowName;
	const char* windowClassName;

	const char* currentFrameBuffer;
};

#endif
