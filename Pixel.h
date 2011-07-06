
#ifndef PIXEL_H_
#define PIXEL_H_

#include "Type.h"

struct PixelBase
{
	PixelBase()
	{
		Reset();
	}
	virtual ~PixelBase(){}

	virtual void Reset() {}

};

template<typename ColorComponentDepth = Intensity, typename zDepth = Depth>
struct Pixel : PixelBase
{
	ColorComponentDepth data[4];
	zDepth z;
	Pixel()
	{
		
	}
	void Reset()
	{
		data[0] = ColorComponentDepth();
		data[1] = ColorComponentDepth();
		data[2] = ColorComponentDepth();
		data[3] = ColorComponentDepth();
		z = (zDepth)INT_MAX;
	}
};

template<>
struct Pixel<Intensity,Depth> : PixelBase
{
	Intensity red;
	Intensity green;
	Intensity blue;
	Intensity alpha;
	Depth z;
	Pixel()
	{
		
	}
	void Reset()
	{
		red = (Intensity)250;
		green = (Intensity)250;
		blue = (Intensity)150;
		alpha = (Intensity)0;
		z = (Depth)INT_MAX;
	}
};

#endif
