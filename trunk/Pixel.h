
#ifndef PIXEL_H_
#define PIXEL_H_

#include "Type.h"
#include <float.h>
#include "Alignment.h"

/*ALIGN_TO(32)*/ struct PixelBase
{
	PixelBase()	: writtenTo(false)
	{
		Reset();
	}
	virtual ~PixelBase(){}
	virtual void Reset() {}
	const bool IsWrittenTo() const
	{
		return writtenTo;
	}

	bool writtenTo;
};

template<typename ColorComponentDepth = Intensity, typename zDepth = Depth, typename int dataSize = 4>
struct Pixel : PixelBase
{
	ALIGN ColorComponentDepth data[dataSize];
	zDepth z;
	Pixel()
	{
		//data = new ColorComponentDepth[size];
	}
	~Pixel()
	{
		//delete data;
	}

	void PutData(ColorComponentDepth&& incData)
	{
		data[0] = std::forward<ColorComponentDepth>(incData);
		writtenTo = true;
	}

	void PutData(ColorComponentDepth& incData)
	{
		data[0] = incData;
		writtenTo = true;
	}

	void Reset()
	{
		for(int i = 0; i < dataSize; ++i)
			data[i] = ColorComponentDepth();
		z = (zDepth)FLT_MAX;
		writtenTo = false;
	}
};

enum ColorIndex : unsigned short
{
	RED = 0,
	GREEN,
	BLUE,
	ALPHA
};

template<>
struct Pixel<Intensity,Depth,4> : PixelBase
{
	ALIGN Intensity color[4];
	Depth z;
	Pixel()
	{
		
	}
	void Reset()
	{
		color[RED] = (Intensity)250;
		color[GREEN] = (Intensity)250;
		color[BLUE] = (Intensity)150;
		color[ALPHA] = (Intensity)0;
		z = (Depth)FLT_MAX;
		writtenTo = false;
	}
};

#endif
