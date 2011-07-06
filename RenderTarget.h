
#ifndef RENDER_TARGET_H_
#define RENDER_TARGET_H_

#include "Constants.h"
#include "FrameBuffer.h"

class RenderTarget
{
public:
	RenderTarget()	:	xDimension(DEFAULT_WIDTH),
						yDimension(DEFAULT_HEIGHT)
	{
		buffer = (FramebufferBase*) new FrameBuffer<>();
	}

	const unsigned int GetColorDepth()
	{
		return buffer->GetColorDepth();
	}

	const float GetWidth() const
	{
		return (float)xDimension;
	}

	const float GetHeight() const
	{
		return (float)yDimension;
	}

	FramebufferBase* GetBuffer()
	{
		return buffer;
	}

	void PutPixel(){}

	const char* FlushBuffer()
	{
		return buffer->FlushBuffer();
	}

	~RenderTarget()
	{
		delete buffer;
	}

private:
	unsigned int xDimension;
	unsigned int yDimension;
	bool active;
	FramebufferBase* buffer; //buffer
};

#endif