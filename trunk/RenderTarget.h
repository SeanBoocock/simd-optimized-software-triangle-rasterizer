
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
		/*buffer = new FrameBuffer<>();*/
	}

	unsigned int GetColorDepth()
	{
		return buffer.GetColorDepth();
	}

	const float GetWidth() const
	{
		return (float)xDimension;
	}

	const float GetHeight() const
	{
		return (float)yDimension;
	}

	FrameBuffer<>* GetBuffer()
	{
		return &buffer;
	}

	void PutPixel(){}

	char* FlushBuffer()
	{
		return buffer.FlushBuffer();
	}

	~RenderTarget()
	{

	}

private:
	unsigned int xDimension;
	unsigned int yDimension;
	bool active;
	FrameBuffer<> buffer; //buffer
};

#endif