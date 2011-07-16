
#ifndef FRAMEBUFFER_BASE_H_
#define FRAMEBUFFER_BASE_H_

#include "Pixel.h"

class FramebufferBase
{
public:
	FramebufferBase(){}
	virtual ~FramebufferBase(){}

	/*virtual void PutPixel(PixelBase&& pixel, unsigned int x, unsigned int y, bool directWrite = false) = 0;*/
	virtual const float GetWidth() = 0;
	virtual const float GetHeight() = 0;
	virtual char* FlushBuffer() = 0;
	virtual unsigned int GetColorDepth() = 0;
	virtual void Reset() = 0;
	virtual void Resize(int xDim, int yDim) = 0;
};

#endif