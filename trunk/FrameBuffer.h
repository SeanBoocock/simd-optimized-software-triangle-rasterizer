
#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_

#include "FramebufferBase.h"
#include "Type.h"
#include "Constants.h"
#include <vector>
#include "Alignment.h"
#include "Pixel.h"
#include "Debugging.h"

template<typename BufferOf = Pixel<Intensity,Depth>, typename ColorComponentDepth = Intensity, typename zDepth = Depth>
class FrameBuffer	:	public FramebufferBase
{
public:
	
	
	FrameBuffer(unsigned int xDimension = DEFAULT_WIDTH,
				unsigned int yDimension = DEFAULT_HEIGHT)	:	xDim(xDimension),
																yDim(yDimension)
	{
		buffer = new BufferOf[bufferSize = xDim * yDim];
		Reset();
	}

	~FrameBuffer()
	{
		if( buffer )
			delete buffer;
	}

	ALIGNED_NEW

	void PutPixel(PixelBase* pixel, unsigned int x, unsigned int y)
	{

	}

	char* FlushBuffer()
	{
		return nullptr;
	}

	unsigned int GetColorDepth()
	{
		return 0;
	}

	void Reset()
	{
		for(unsigned int i = 0; i < bufferSize; ++i)
			buffer[i].Reset();
	}

	void Resize(int xDim, int yDim)
	{

	}

private:
	BufferOf* buffer;
	unsigned int bufferSize;
	unsigned int xDim;
	unsigned int yDim;
};

template<typename ColorComponentDepth, typename zDepth>
class FrameBuffer<Pixel<Intensity,Depth>, ColorComponentDepth, zDepth>	:	public FramebufferBase
{
public:
	
	
	FrameBuffer(unsigned int xDimension = DEFAULT_WIDTH,
				unsigned int yDimension = DEFAULT_HEIGHT)	:	xDim(xDimension),
																yDim(yDimension),
																bufferToDisplay(nullptr)
	{
		buffer = new Pixel<Intensity,Depth>[bufferSize = xDim * yDim];
		Reset();
	}

	~FrameBuffer()
	{
		if( buffer )
			delete buffer;
	}

	ALIGNED_NEW

	virtual const float GetWidth()
	{
		return (float) xDim;
	}

	virtual const float GetHeight()
	{
		return (float) yDim;
	}

	void PutPixel(PixelBase* pixel, unsigned int x, unsigned int y)
	{
		++pixelsPassedLEETest;
		if( ((Pixel<Intensity,Depth>*)pixel)->z < buffer[ y * xDim + x ].z )
		{
			++pixelsPassedZTest;
			buffer[ y * xDim + x ] = *((Pixel<Intensity,Depth>*)pixel);
		}
	}

	//PARALLEL: can use parallel for here ot do this
	char* FlushBuffer()
	{
		bufferToDisplay = new char[bufferSize * 3];
		for(unsigned int i = 0, j = -1; i < bufferSize; ++i)
		{
			bufferToDisplay[++j] = (char) (0xFFFFFFFF & buffer[i].red);
			bufferToDisplay[++j] = (char) (0xFFFFFFFF & buffer[i].green);
			bufferToDisplay[++j] = (char) (0xFFFFFFFF & buffer[i].blue);
		}
		return bufferToDisplay;
	}

	unsigned int GetColorDepth()
	{
		return sizeof(ColorComponentDepth) * 8 * 3; //size of data type for each color component times 8 bits per byte time three components in buffer
	}

	void Reset()
	{
		for(unsigned int i = 0; i < bufferSize; ++i)
			buffer[i].Reset();
	}

	void Resize(int xDim, int yDim)
	{

	}

private:
	Pixel<Intensity,Depth>* buffer;
	unsigned int bufferSize;
	unsigned int xDim;
	unsigned int yDim;
	char* bufferToDisplay;
};

#endif