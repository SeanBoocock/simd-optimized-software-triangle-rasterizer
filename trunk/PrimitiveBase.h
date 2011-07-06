
#ifndef PRIMITIVE_BASE_H_
#define PRIMITIVE_BASE_H_

#include "MatrixMath.h"
#include <math.h>

class Renderer;

#if 1
static int PrimitiveCount = 0;
#endif

template<typename T>
struct PixelBounds
{
	T minX;
	T minY;
	T maxX;
	T maxY;
};

class PrimitiveBase
{
public:
	PrimitiveBase()
	{
		id = PrimitiveCount++;
	}

	virtual ~PrimitiveBase(){}

	virtual void ExecutePipeline() = 0;

	virtual void TransformVertices(const Math::Matrix4X4 &transform) = 0;

	virtual unsigned int ClipVertices(const Math::Vector4 &min, const Math::Vector4 &max) = 0;
	
	virtual void SortVertices() = 0;

	virtual void LEETest(FramebufferBase* buffer) = 0;

	virtual void GenerateBounds(const int* xOrder, const int* yOrder) = 0;

	const unsigned int ID()
	{
		return id;
	}

	void SetPixelBounds(float minX, float minY, float maxX, float maxY)
	{
		pixelBounds.minX = (int) floor(minX);
		pixelBounds.minY = (int) floor(minY);
		pixelBounds.maxX = (int) ceil(maxX);
		pixelBounds.maxY = (int) ceil(maxY);
	}

protected:
	PixelBounds<int> pixelBounds;
	unsigned int id;

};

typedef unsigned int (Renderer::*StagePtr)(PrimitiveBase* primitive);

#endif