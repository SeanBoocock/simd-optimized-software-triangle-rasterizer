
#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

#include "Buffer.h"
#include "Primitive.h"
#include "Alignment.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(unsigned int size = 0)	:	buffer(nullptr),
											totalSize(size),
											currentSize(0)
	{
		if(totalSize)
		{
			buffer = new PrimitiveBase*[totalSize];
		}
	}

	~VertexBuffer()
	{
		if(buffer)
		{
			for(unsigned int i = 0; i < totalSize; ++i)
				delete buffer[i];
		}
		delete buffer;
	}

	ALIGNED_NEW

	PrimitiveBase* At(unsigned int index)
	{
		return (index < currentSize && buffer != nullptr && index >= 0) ? buffer[index] : nullptr;
	}

	const unsigned int Size() const
	{
		return currentSize;
	}

	void PushBack(PrimitiveBase* toAdd)
	{
		if(currentSize < totalSize)
			buffer[currentSize++] = toAdd;
	}

	void FillBuffer(PrimitiveBase** input, unsigned int numberOfElements)
	{
		currentSize = numberOfElements;
		memcpy(buffer, input, currentSize * sizeof(PrimitiveBase));
	}

	PrimitiveBase** FetchBuffer(unsigned int &count)
	{
		count = currentSize;
		return buffer;
	}

	void Reset()
	{

	}

private:
	unsigned int totalSize;
	unsigned int currentSize;
	PrimitiveBase** buffer;
};

#endif