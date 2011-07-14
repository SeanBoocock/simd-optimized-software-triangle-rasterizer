
#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_

#include "FramebufferBase.h"
#include "Type.h"
#include "Constants.h"
#include <vector>
#include "Alignment.h"
#include "Pixel.h"
#include "Debugging.h"
#include "InterlockedZBufferExchange.h"


template<typename BufferOf = Pixel<Intensity,Depth>, typename ColorComponentDepth = Intensity, typename zDepth = Depth>
class FrameBuffer	:	public FramebufferBase
{
public:
	
	
	FrameBuffer(unsigned int xDimension = DEFAULT_WIDTH,
				unsigned int yDimension = DEFAULT_HEIGHT)	:	xDim(xDimension),
																yDim(yDimension)
	{
		buffer = new BufferOf*[bufferSize = xDim * yDim];
		InitializeSRWLock(&zLock);
		for(unsigned int i = 0; i < bufferSize; ++i)
			buffer[i] = new BufferOf();
	}

	~FrameBuffer()
	{
		if( buffer )
			delete [] buffer;
	}

	ALIGNED_NEW

	const float GetWidth()
	{
		return (float) xDim;
	}

	const float GetHeight()
	{
		return (float) yDim;
	}

	void PutPixel(PixelBase* pixel, unsigned int x, unsigned int y, bool directWrite = false)
	{
		if(directWrite)
		{
			delete buffer[ y * xDim + x ];
			buffer[ y * xDim + x ] = static_cast<BufferOf*>(pixel);
			return;
		}
		
		BufferOf* oldVal = buffer[ y * xDim + x ];
		if(static_cast<Pixel<Math::Vector4,Depth,1>*>(pixel)->z < static_cast<Pixel<Math::Vector4,Depth,1>*>(oldVal)->z)
		{
			AcquireSRWLockExclusive(&zLock);
			if(static_cast<Pixel<Math::Vector4,Depth,1>*>(pixel)->z < static_cast<Pixel<Math::Vector4,Depth,1>*>(oldVal)->z)
			{
				delete oldVal;
				buffer[ y * xDim + x ] = static_cast<Pixel<Math::Vector4,Depth,1>*>(pixel);
			}
			ReleaseSRWLockExclusive(&zLock);
		}
		/*if( InterlockedZBufferExchange(&buffer[ y * xDim + x ],(BufferOf*)pixel) )
		{
			delete oldVal;
		}*/
	}

	void PutPixel(PixelBase* pixel, unsigned int val, bool directWrite = false)
	{
		if(directWrite)
		{
			delete buffer[ val ];
			buffer[ val ] = static_cast<BufferOf*>(pixel);
			return;
		}
		
		BufferOf* oldVal = buffer[ val ];

		if( InterlockedZBufferExchange(&buffer[ val ],(BufferOf*)pixel) )
		{
			delete oldVal;
		}
	}

	const PixelBase* GetPixel(unsigned int x, unsigned int y)
	{
		return buffer[ y * xDim + x ];
	}

	const PixelBase* GetPixel(unsigned int val)
	{
		return buffer[ val ];
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
			buffer[i]->Reset();
	}

	void Resize(int xDim, int yDim)
	{

	}

private:
	BufferOf** buffer;
	unsigned int bufferSize;
	unsigned int xDim;
	unsigned int yDim;
	SRWLOCK zLock;
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
		buffer = new Pixel<Intensity,Depth>*[bufferSize = xDim * yDim];
		for(unsigned int i = 0; i < bufferSize; ++i)
			buffer[i] = new Pixel<Intensity,Depth>();
	}

	~FrameBuffer()
	{
		if( buffer )
			delete [] buffer;
	}

	ALIGNED_NEW

	const float GetWidth()
	{
		return (float) xDim;
	}

	const float GetHeight()
	{
		return (float) yDim;
	}

	void PutPixel(PixelBase* pixel, unsigned int x, unsigned int y, bool directWrite = false)
	{
		if(directWrite)
		{
			delete buffer[ y * xDim + x ];
			buffer[ y * xDim + x ] = (Pixel<Intensity,Depth>*)pixel;
			return;
		}
		if( buffer[ y * xDim + x ] )
		{
			Pixel<Intensity,Depth,4>* oldVal = buffer[ y * xDim + x ];

			if( InterlockedZBufferExchange(&buffer[ y * xDim + x ],(Pixel<Intensity,Depth,4>*)pixel) )
			{
				delete oldVal;
			}
		}
	}

	void PutPixel(PixelBase* pixel, unsigned int val, bool directWrite = false)
	{
		if(directWrite)
		{
			delete buffer[ val ];
			buffer[ val ] = (Pixel<Intensity,Depth>*)pixel;
			return;
		}
		if( buffer[ val ] )
		{
			Pixel<Intensity,Depth,4>* oldVal = buffer[ val ];

			if( InterlockedZBufferExchange(&buffer[ val ],(Pixel<Intensity,Depth,4>*)pixel) )
			{
				delete oldVal;
			}
		}
	}

	//PARALLEL: can use parallel for here ot do this
	char* FlushBuffer()
	{
		bufferToDisplay = new char[bufferSize * 3];
		for(unsigned int i = 0, j = -1; i < bufferSize; ++i)
		{
			bufferToDisplay[++j] = (unsigned char) (buffer[i]->color[BLUE]);
			bufferToDisplay[++j] = (unsigned char) (buffer[i]->color[GREEN]);
			bufferToDisplay[++j] = (unsigned char) (buffer[i]->color[RED]);
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
			buffer[i]->Reset();
	}

	void Resize(int xDim, int yDim)
	{

	}

private:
	Pixel<Intensity,Depth,4>** buffer;
	unsigned int bufferSize;
	unsigned int xDim;
	unsigned int yDim;
	char* bufferToDisplay;
};

#endif