
#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

HRESULT Window::Initialize()
{
	return ( (RegisterWindow() == S_OK) && (CreateWinWindow() == S_OK) ) ;
}

HRESULT Window::RegisterWindow()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = windowClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Window::CreateWinWindow()
{
	hWnd = CreateWindow(	windowClassName, windowName, WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance,
							NULL );
	if(hWnd)
		deviceContext = GetDC(hWnd);

	return (hWnd) ? ( ShowWindow(hWnd,SW_SHOW) ? S_OK : E_FAIL ) : E_FAIL;
}

bool Window::MessagePump()
{
	MSG msg = {0};

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (WM_QUIT != msg.message) ? true : false;
}

HRESULT Window::Present(RenderTarget* target)
{
	if(!target)
		return E_FAIL;

	HDC memoryHdc;
	HBITMAP bitmap;

	if(currentFrameBuffer)
		delete currentFrameBuffer;

    memoryHdc = ::CreateCompatibleDC(deviceContext);

	// Display the current image
    char buffer[sizeof(BITMAPINFO)];
    BITMAPINFO* binfo = (BITMAPINFO*)buffer;
    memset(binfo, 0, sizeof(BITMAPINFOHEADER));
    binfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    
	// Create the bitmap
    BITMAPINFOHEADER* bih = &binfo->bmiHeader;
	bih->biBitCount = target->GetColorDepth();
	bih->biWidth = width;
	bih->biHeight = height;
    bih->biPlanes = 1;
    bih->biCompression = BI_RGB;
    bih->biSizeImage = 0;
    
    bitmap = CreateDIBSection(memoryHdc , binfo, 0, 0, 0, DIB_RGB_COLORS);

    int colors = DIB_RGB_COLORS;
    
    ::SelectObject(memoryHdc , bitmap);
	binfo->bmiHeader.biBitCount = 0;
	GetDIBits(memoryHdc , bitmap, 0, 0, 0, binfo, colors);
    binfo->bmiHeader.biBitCount = target->GetColorDepth();
    binfo->bmiHeader.biHeight = -abs(binfo->bmiHeader.biHeight);
	currentFrameBuffer = target->FlushBuffer();
    SetDIBits(memoryHdc , bitmap, 0, height, currentFrameBuffer, binfo, colors);

    ::SetStretchBltMode(deviceContext, COLORONCOLOR);
    ::BitBlt(deviceContext, 0, 0, width, height, memoryHdc, 0, 0, SRCCOPY);
    ::DeleteDC(memoryHdc);
    DeleteObject(bitmap); 
	return S_OK;
}