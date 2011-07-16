
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Vector.h"
#include "Pixel.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define TOP_LEFT(index) (int)( index[0] + ( index[1] * xDim ) )
#define TOP_RIGHT(index) (int)( index[0] + 1 + ( index[1] * xDim ) )
#define BOTTOM_LEFT(index) (int)( index[0] + ( ( index[1] +1 )* xDim ) )
#define BOTTOM_RIGHT(index) (int)( index[0] + 1 + ( ( index[1] +1 )  * xDim ) )

class Texture
{
public:
	Texture(const char* incFileName = nullptr)	:	fileName(incFileName),
													textureData(nullptr),
													xDim(0),
													yDim(0)
	{

	}

	~Texture()
	{
		if(textureData) delete textureData;
	}

	Math::Vector4 SampleUV(Math::Vector4 &uvs)
	{
		if( ( _mm_movemask_ps( _mm_cmplt_ps( uvs, Math::zero )  ) & ( VERT_Y | VERT_X  ) ) > 0 ) // either u or v (or both) is less than zero
		{
			if( ( _mm_movemask_ps( _mm_cmpgt_ps( uvs, Math::ident )  ) & ( VERT_Y | VERT_X  ) ) > 0 ) // either u or v (or both) is greater than 1
			{
				Math::Vector4 floor, fractional;
				Math::ModF(uvs,floor,fractional);
				uvs = fractional;
			}
			ALIGN float indices[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			Math::Vector4 scaledUVs = _mm_mul_ps(uvs, dimensions);
			Math::Vector4 floor, fractional, offsetFractional, coefficients, tempCoefficients, color;

			Math::ModF(scaledUVs,floor,fractional);
			_mm_store_ps(indices,floor);

			offsetFractional = _mm_sub_ps(Math::ident4,fractional);
			floor = _mm_shuffle_ps(fractional, offsetFractional, _MM_SHUFFLE(1, 1, 1, 1)); // { vOffset, vOffset, (1-vOffset), (1-vOffset) };
			offsetFractional = _mm_shuffle_ps(fractional, offsetFractional, _MM_SHUFFLE(0, 0, 0, 0)); // { uOffset, uOffset, (1-uOffset), (1-uOffset) };
			offsetFractional = _mm_shuffle_ps(offsetFractional, offsetFractional, _MM_SHUFFLE(2, 0, 2, 0)); // { uOffset, (1-uOffset), uOffset, (1-uOffset) };
		
			color = Math::zero;
			coefficients = _mm_mul_ps(floor,offsetFractional); // { uOffset * vOffset, (1-uOffset) * vOffset, uOffset *(1-vOffset), (1-uOffset) * (1-vOffset) };
			tempCoefficients = _mm_shuffle_ps(coefficients, coefficients, _MM_SHUFFLE(0, 0, 0, 0)); // { uOffset * vOffset, uOffset * vOffset, uOffset * vOffset, uOffset * vOffset }
			tempCoefficients = _mm_mul_ps(tempCoefficients, textureData[ BOTTOM_RIGHT(indices) ].data[0]);
			color = _mm_add_ps(color,tempCoefficients);

			tempCoefficients = _mm_shuffle_ps(coefficients, coefficients, _MM_SHUFFLE(1, 1, 1, 1)); // { uOffset * vOffset, uOffset * vOffset, uOffset * vOffset, uOffset * vOffset }
			tempCoefficients = _mm_mul_ps(tempCoefficients, textureData[ BOTTOM_LEFT(indices) ].data[0]);
			color = _mm_add_ps(color,tempCoefficients);

			tempCoefficients = _mm_shuffle_ps(coefficients, coefficients, _MM_SHUFFLE(2, 2, 2, 2)); // { uOffset * vOffset, uOffset * vOffset, uOffset * vOffset, uOffset * vOffset }
			tempCoefficients = _mm_mul_ps(tempCoefficients, textureData[ TOP_RIGHT(indices) ].data[0]);
			color = _mm_add_ps(color,tempCoefficients);

			tempCoefficients = _mm_shuffle_ps(coefficients, coefficients, _MM_SHUFFLE(3, 3, 3, 3)); // { uOffset * vOffset, uOffset * vOffset, uOffset * vOffset, uOffset * vOffset }
			tempCoefficients = _mm_mul_ps(tempCoefficients, textureData[ TOP_LEFT(indices) ].data[0]);
			return color = _mm_add_ps(color,tempCoefficients);
		}
		return Math::zero;
		/*
		topLeft = cornerU + cornerV*(xs);
		topRight = cornerU + 1 + cornerV*(xs);
		bottomLeft = cornerU + (cornerV+1)*(xs);
		bottomRight = cornerU + 1 + (cornerV+1)*(xs);

		for(int i = 0; i < 3; i++)
		{
			color[i] =	uOffset*vOffset*image[bottomRight][i] + 
						(1-uOffset)*vOffset*image[bottomLeft][i] + 
						uOffset*(1-vOffset)*image[topRight][i] + 
						(1-uOffset)*(1-vOffset)*image[topLeft][i];				
		}
		*/
	}

	bool LoadTexture(const char* incFileName = nullptr)
	{
		FILE* textureFile = nullptr;
		char garbage[8];
		char garbageChar;

		char* pixelBuffer = nullptr;
		ALIGN float tempPixel[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Math::Vector4 toAssignToPixel;

		if(incFileName)
		{
			fileName = incFileName;
		}

		if( fopen_s(&textureFile,fileName, "rb") )
		{
			printf_s("Could not open file %s.  Texture not loaded.\n", fileName);
			return false;
		}

		fscanf_s(textureFile,"%s %d %d %c", garbage, &xDim, &yDim, &garbageChar);
		
		if(xDim > 0 && yDim > 0)
		{
			tempPixel[0] = (float)xDim - 1.0f;
			tempPixel[1] = (float)yDim - 1.0f;
			dimensions = Math::LoadVector4Aligned(tempPixel);
			tempPixel[3] = 0.0f;
			try
			{
				textureData = new Pixel<Math::Vector4, Depth, 1>[xDim*yDim];
				pixelBuffer = new char[3*xDim*yDim];
			}
			catch(...)
			{
				printf_s("Texture buffer error during allocation.  Texture not loaded.\n", fileName);
				fclose(textureFile);
				return false;
			}

			if( fread(pixelBuffer,sizeof(char)*3*xDim*yDim,3*xDim*yDim,textureFile) < 3*xDim*yDim )
			{
				printf_s("Texture error during file reading.  Texture not loaded.\n", fileName);
				fclose(textureFile);
				delete textureData;
				delete pixelBuffer;
				return false;
			}

			for(unsigned int i = 0; i < xDim*yDim; ++i)
			{
				tempPixel[0] = pixelBuffer[3*i] / 255.0f;
				tempPixel[1] = pixelBuffer[3*i+1] / 255.0f;
				tempPixel[2] = pixelBuffer[3*i+2] / 255.0f;
				toAssignToPixel = Math::LoadVector4Aligned(tempPixel);
				textureData[i].PutData(std::forward<Math::Vector4 >(toAssignToPixel));
			}
		}
	}

private:
	Pixel<Math::Vector4, Depth, 1>* textureData;

	unsigned int xDim;
	unsigned int yDim;
	Math::Vector4 dimensions;
	const char* fileName;
};

#endif