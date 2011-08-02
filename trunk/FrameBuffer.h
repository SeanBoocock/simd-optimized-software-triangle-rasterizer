
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
#include "tbb/parallel_for.h"

template<typename BufferOf = Pixel<Intensity,Depth>, typename ColorComponentDepth = Intensity, typename zDepth = Depth>
class FrameBuffer	:	public FramebufferBase
{
public:

	FrameBuffer(unsigned int xDimension = DEFAULT_WIDTH,
				unsigned int yDimension = DEFAULT_HEIGHT)	:	xDim(xDimension),
																yDim(yDimension)
	{
		buffer = new BufferOf[bufferSize = xDim * yDim];
		InitializeSRWLock(&zLock);
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

	void PutPixel(BufferOf&& pixel, unsigned int x, unsigned int y, bool directWrite = false)
	{
		if(directWrite)
		{
			buffer[ y * xDim + x ] = std::forward<BufferOf>(pixel);
			return;
		}
		
		//if(pixel.z < buffer[ y * xDim + x ].z)
		//{
		//	AcquireSRWLockExclusive(&zLock);
		//	if(pixel.z < buffer[ y * xDim + x ].z)
		//	{
		//		/*delete oldVal;*/
		//		buffer[ y * xDim + x ] = std::forward<BufferOf>(pixel);
		//	}
		//	ReleaseSRWLockExclusive(&zLock);
		//}
		if( InterlockedZExchange(&buffer[ y * xDim + x ],&pixel) )
		{
			buffer[ y * xDim + x ] = std::forward<BufferOf>(pixel);
		}
	}

	void PutPixel(BufferOf&& pixel, unsigned int val, bool directWrite = false)
	{
		if(directWrite)
		{
			/*delete buffer[ val ];*/
			buffer[ val ] = std::forward<BufferOf>(pixel);
			return;
		}
		
		//if(pixel.z < buffer[ val ].z)
		//{
		//	AcquireSRWLockExclusive(&zLock);
		//	if(pixel.z < buffer[ val ].z)
		//	{
		//		/*delete oldVal;*/
		//		buffer[ val ] = std::forward<BufferOf>(pixel);
		//	}
		//	ReleaseSRWLockExclusive(&zLock);
		//}

		if( InterlockedZExchange(&buffer[ y * xDim + x ],&pixel) )
		{
			buffer[ y * xDim + x ] = std::forward<BufferOf>(pixel);
		}
	}

	const BufferOf GetPixel(unsigned int x, unsigned int y)
	{
		return buffer[ y * xDim + x ];
	}

	const BufferOf GetPixel(unsigned int val)
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
		tbb::parallel_for(0,(int)bufferSize,[&](int i)
		{
			buffer[i].Reset();
		});
	}

	void Resize(int xDim, int yDim)
	{

	}

private:
	BufferOf* buffer;
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
		buffer = new Pixel<Intensity,Depth>[bufferSize = xDim * yDim];
		InitializeSRWLock(&zLock);
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

	void PutPixel(Pixel<Intensity,Depth>&& pixel, unsigned int x, unsigned int y, bool directWrite = false)
	{
		if(directWrite)
		{
			/*delete buffer[ y * xDim + x ];*/
			buffer[ y * xDim + x ] = std::forward<Pixel<Intensity,Depth>>(pixel);
			return;
		}

		//if(pixel.z < buffer[ y * xDim + x ].z)
		//{
		//	AcquireSRWLockExclusive(&zLock);
		//	if(pixel.z < buffer[ y * xDim + x ].z)
		//	{
		//		/*delete oldVal;*/
		//		buffer[ y * xDim + x ] = std::forward<Pixel<Intensity,Depth>>(pixel);
		//	}
		//	ReleaseSRWLockExclusive(&zLock);
		//}

		if( InterlockedZExchange(&buffer[ y * xDim + x ],&pixel) )
		{
			buffer[ y * xDim + x ] = std::forward<Pixel<Intensity,Depth>>(pixel);
		}
	}

	void PutPixel(Pixel<Intensity,Depth>&& pixel, unsigned int val, bool directWrite = false)
	{
		if(directWrite)
		{
			buffer[ val ] = std::forward<Pixel<Intensity,Depth>>(pixel);
			return;
		}

		if( InterlockedZExchange(&buffer[ val ],&pixel) )
		{
			buffer[ val ] = std::forward<Pixel<Intensity,Depth>>(pixel);
		}

		//if(pixel.z < buffer[ val ].z)
		//{
		//	AcquireSRWLockExclusive(&zLock);
		//	if(pixel.z < buffer[ val ].z)
		//	{
		//		/*delete oldVal;*/
		//		buffer[ val ] = std::forward<Pixel<Intensity,Depth>>(pixel);
		//	}
		//	ReleaseSRWLockExclusive(&zLock);
		//}

	}

	//PARALLEL: can use parallel for here ot do this
	char* FlushBuffer()
	{
		bufferToDisplay = new char[bufferSize * 3];
		for(unsigned int i = 0, j = -1; i < bufferSize; ++i)
		{
			bufferToDisplay[++j] = (unsigned char) (buffer[i].color[BLUE]);
			bufferToDisplay[++j] = (unsigned char) (buffer[i].color[GREEN]);
			bufferToDisplay[++j] = (unsigned char) (buffer[i].color[RED]);
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
	SRWLOCK zLock;
};

#endif