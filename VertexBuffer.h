
#ifndef VERTEX_BUFFER_H_
#define VERTEX_BUFFER_H_

//#include "Buffer.h"
#include "Primitive.h"
#include "Alignment.h"

class VertexBuffer 
{
public:
	VertexBuffer(unsigned int size = 0)	:	buffer(nullptr),
											totalSize(size),
											currentSize(0)
	{
		if(totalSize)
		{
			buffer = new Primitive<>[totalSize];
		}
	}

	~VertexBuffer()
	{
		/*if(buffer)
		{
			for(unsigned int i = 0; i < totalSize; ++i)
				delete buffer[i];
		}*/
		delete buffer;
	}

	ALIGNED_NEW

	Primitive<>* At(unsigned int index)
	{
		return (index < currentSize && buffer != nullptr && index >= 0) ? &buffer[index] : nullptr;
	}

	const unsigned int Size() const
	{
		return currentSize;
	}

	void PushBack(Primitive<>&& toAdd)
	{
		if(currentSize < totalSize)
			buffer[currentSize++] = std::forward<Primitive<>>(toAdd);
	}

	void FillBuffer(PrimitiveBase** input, unsigned int numberOfElements)
	{
		currentSize = numberOfElements;
		memcpy(buffer, input, currentSize * sizeof(PrimitiveBase));
	}

	Primitive<>* FetchBuffer(unsigned int &count)
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
	Primitive<>* buffer;
};

#endif