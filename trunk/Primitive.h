
#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "Pipeline.h"
#include "PrimitiveBase.h"
#include "Alignment.h"

enum ComponentComparison : unsigned short
{
	VERT_X = 0x1,
	VERT_Y = 0x2,
	VERT_Z = 0x4,
	VERT_W = 0x8,
	VERT_ALL = VERT_X | VERT_Y | VERT_Z | VERT_W
};

template<typename DataType = Math::Vector4, unsigned short NumVertices = 3>
class Primitive: public PrimitiveBase
{
public:
	Primitive(DataType* incVertices = nullptr)	:	currentNumberVertices(0)
	{
		if(incVertices)
			AddVertices(incVertices);
		pipeline = new Pipeline<>();
	}

	~Primitive(){}

	ALIGNED_NEW

	void ExecutePipeline()
	{
		pipeline->Execute(this);
	}

	void TransformVertices(const Math::Matrix4X4 &transform)
	{

	}

	void SortVertices() 
	{

	}

	void GenerateBounds(const int* xOrder, const int* yOrder)
	{

	}

	void LEETest(FramebufferBase* buffer)
	{

	}

	unsigned int ClipVertices(const Math::Vector4 &min, const Math::Vector4 &max)
	{
		return RENDERER_SUCCESS;
	}

	void AddVertices(DataType* incVertices)
	{
		memcpy(vertices,incVertices,sizeof(DataType)*NumVertices);
		currentNumberVertices = NumVertices;
	}

	void AddVertex(const DataType &vertex)
	{
		if(currentNumberVertices < NumVertices)
			vertices[currentNumberVertices++] = vertex;
	}

private:
	DataType vertices[NumVertices];
	unsigned short currentNumberVertices;
	PipelineBase* pipeline;
};


template<typename DataType>
class Primitive<DataType,3>: public PrimitiveBase
{
public:
	Primitive(DataType* incVertices = nullptr)	:	currentNumberVertices(0)
	{
		if(incVertices)
			AddVertices(incVertices);
		pipeline = new Pipeline<>();
	}

	~Primitive(){}

	ALIGNED_NEW

	void ExecutePipeline()
	{
		pipeline->Execute(this);
	}

	void TransformVertices(const Math::Matrix4X4 &transform)
	{
		Math::Matrix4X4 toTransform = Math::Matrix4X4(vertices[0],vertices[1],vertices[2],Math::ident0);
		//
		Math::Matrix4X4 transformed = Math::Matrix4X4Multiply( transform, toTransform, false );
		Math::Transpose( transformed );
		Math::ScaleForW( transformed.rows[0] );
		Math::ScaleForW( transformed.rows[1] );
		Math::ScaleForW( transformed.rows[2] );
		
		vertices[0] = transformed.rows[0];
		vertices[1] = transformed.rows[1];
		vertices[2] = transformed.rows[2];
	}

	unsigned int ClipVertices(const Math::Vector4 &min, const Math::Vector4 &max)
	{
		Math::Vector4 comparison = _mm_cmpge_ps( vertices[0], min );
		/*comparison = _mm_and_ps( comparison, _mm_cmpge_ps( vertices[0], min ) );*/
		comparison = _mm_and_ps( comparison, _mm_cmpge_ps( vertices[1], min ) );
		comparison = _mm_and_ps( comparison, _mm_cmpge_ps( vertices[2], min ) );

		comparison = _mm_and_ps( comparison, _mm_cmple_ps( vertices[0], max ) );
		comparison = _mm_and_ps( comparison, _mm_cmple_ps( vertices[1], max ) );
		comparison = _mm_and_ps( comparison, _mm_cmple_ps( vertices[2], max ) );
		return (_mm_movemask_ps(comparison) & VERT_ALL) ? RENDERER_SUCCESS : RENDERER_PRIMITIVE_CLIPPED;
	}

	void SortVertices() 
	{
		int yOrder[3] = { 0, 0, 0 }; //Y order from largest to smallest
		int xOrder[3] = { 0, 0, 0 };
		//Sort vertically
		//if v[0] was > v[1] then if v[0] is also > v[2], keep v[0] as v[0]; otherwise v[0] = v[2];
		SortByVertexComponent(yOrder,VERT_Y);
		SortByVertexComponent(xOrder,VERT_X);

		GenerateBounds(xOrder,yOrder);

		//Find the vertex between the lower two that is on the left.  If that order is different from yOrder then swap values
		if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[ yOrder[1] ], vertices[ yOrder[2] ] )  ) & VERT_X )
		{
			int swap = yOrder[1];
			yOrder[1] = yOrder[2];
			yOrder[2] = swap;
		}

		//Do swaps
		if(yOrder[0] != 0)
		{
			Math::Vector4 swap = vertices[0];
			vertices[0] = vertices[ yOrder[0] ];
			if(yOrder[1] != 0)
			{
				vertices[1] = vertices[ yOrder[1] ];
				vertices[2] = swap;
			}
			else
			{
				vertices[1] = swap;
				vertices[2] = vertices[ yOrder[2] ];
			}
		}
		else
		{
			if(yOrder[1] != 1)
			{
				Math::Vector4 swap = vertices[1];
				vertices[1] = vertices[ yOrder[1] ];
				vertices[2] = swap;
			}
		}
	}

	void SortByVertexComponent(int* orderOut, ComponentComparison comp)
	{
		if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[0], vertices[1] ) ) & comp ) // v[0].y > v[1].y = true
		{
			if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[0], vertices[2] )  ) & comp ) // v[0].y > v[2].y = true
			{
				orderOut[0] = 0;
				if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[1], vertices[2] )  ) & comp )
				{
					orderOut[1] = 1;
					orderOut[2] = 2;
				}
				else
				{
					orderOut[1] = 2;
					orderOut[2] = 1;
				}
			}
			else
			{
				orderOut[0] = 2;
				orderOut[1] = 0;
				orderOut[2] = 1;
				
			}
		}
		else // v[0].y > v[1].y = false
		{
			if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[1], vertices[2] )  ) & comp ) // v[1].y > v[2].y = true && v[0].y > v[1].y = false
			{
				orderOut[0] = 1;
				if( _mm_movemask_ps( _mm_cmpgt_ps( vertices[0], vertices[2] )  ) & comp )
				{
					orderOut[1] = 0;
					orderOut[2] = 2;
				}
				else
				{
					orderOut[1] = 2;
					orderOut[2] = 0;
				}
			}
			else // v[1].y > v[2].y = false && v[0].y > v[1].y = false
			{
				orderOut[0] = 2;
				orderOut[1] = 1;
				orderOut[2] = 0;
			}
		}
	}

	void GenerateBounds(const int* xOrder, const int* yOrder)
	{
		ALIGN float storeYMin[4];
		ALIGN float storeYMax[4];
		float minX, maxX;

		_mm_store_ss(&minX, vertices[ xOrder[2] ] );
		_mm_store_ss(&maxX, vertices[ xOrder[0] ] );
		_mm_store_ps(storeYMin, vertices[ yOrder[2] ] );
		_mm_store_ps(storeYMax, vertices[ yOrder[0] ] );

		SetPixelBounds(minX,storeYMin[1],maxX,storeYMax[1]);
	}

	void GenerateBarycentric(Math::Vector4 &output)
	{
		/*
		GzCoord barycentricCoord = {0.0,0.0,0.0};
	float determinant = (vertices[0][0] - vertices[2][0])*(vertices[1][1] - vertices[2][1])
		-(vertices[0][1] - vertices[2][1])*(vertices[1][0] - vertices[2][0]);

	v[0].z * barycentricCoord[0] = ((vertices[1][1]-vertices[2][1])*(x - vertices[2][0])+(vertices[2][0]-vertices[1][0])*(y-vertices[2][1]))/determinant;
	v[1].z * barycentricCoord[1] = ((vertices[2][1]-vertices[0][1])*(x - vertices[2][0])+(vertices[0][0]-vertices[2][0])*(y-vertices[2][1]))/determinant;
	v[2].z * barycentricCoord[2] = 1 - barycentricCoord[0] - barycentricCoord[1];
		*/
		Math::Vector4 temp = _mm_sub_ps(vertices[0],vertices[2]);
		Math::Vector4 temp1 = _mm_sub_ps(vertices[1],vertices[2]);
		temp1 =  _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 3, 0, 1));
		Math::Vector4 determinant = _mm_mul_ps(temp,temp1);
		determinant = _mm_hsub_ps(determinant,determinant);//{determinant,0,determinant,0};

		Math::Vector4 temp2 = _mm_sub_ps(vertices[2],vertices[0]);

	}

	void GenerateInterpolationPlane(Math::Vector4 &output)
	{
		//General plane equation: Ax + By + Cz + D = 0
		Math::Vector4 edgeOne = _mm_sub_ps(vertices[0],vertices[1]);
		Math::Normalize( edgeOne );
		Math::Vector4 edgeTwo = _mm_sub_ps(vertices[0],vertices[2]);
		Math::Normalize( edgeTwo );

		//Reusing edge.  Cross product will provide A,B,C coefficients.
		output = edgeOne = Math::Vec3CrossVec3(edgeOne,edgeTwo);//{ A, B, C, 0 }

		//Solving for D.  D = -Ax - By - Cz.  Reusing edgeTwo.
		edgeOne = _mm_sub_ps(Math::zero,edgeOne); //{ -A, -B, -C, 0 }
		edgeTwo = _mm_mul_ps( edgeOne, vertices[0] ); // { -A*(x0), -B*(y0), -C*(z0), 0 }
		edgeTwo = _mm_hadd_ps( edgeTwo, edgeTwo );  // { -A*(x0)- B*(y0), -C*(z0), -A*(x0)- B*(y0), -C*(z0) }
		edgeTwo = _mm_hadd_ps( edgeTwo, edgeTwo ); // { -A*(x0) - B*(y0) - C*(z0), -A*(x0) - B*(y0) - C*(z0), -A*(x0) - B*(y0) - C*(z0), -A*(x0) - B*(y0) - C*(z0)  D is now in least significant word

		edgeTwo = _mm_insert_ps(Math::zero,edgeTwo,_MM_MK_INSERTPS_NDX(0,1,0)); // { 0, D, 0, 0 };
		edgeOne = _mm_shuffle_ps(edgeOne, edgeOne, _MM_SHUFFLE(3, 1, 3, 0)); // { -A, 0, -B , 0 }
		edgeOne = _mm_sub_ps(edgeOne, edgeTwo); // {-A,-D,-B,0 } 
		edgeTwo = _mm_shuffle_ps(output, output, _MM_SHUFFLE(2, 2, 2, 2)); // {C,C,C,C}
		
		output = _mm_div_ps(edgeOne,edgeTwo); // 0 = { -A/C, - D/C, -B/C, 0};
	}

	void LEETest(FramebufferBase* buffer)
	{
		//Setup 
		Math::Vector4 planeForInterpolation;
		GenerateInterpolationPlane(planeForInterpolation);
		
		/*
		*Edge Equation:  E(x,y) = dY (x-X) - dX (y-Y) 
			= 0 for points (x,y) on line
			= + for points in half-plane to right/below line
			= - for points in half-plane to left/above line
			*/

		//Saving/packing the base of the vectors for the test into variables
		Math::Vector4 vertBaseOneTwo = _mm_shuffle_ps(vertices[0], vertices[1], _MM_SHUFFLE(1, 0, 1, 0));
		Math::Vector4 vertBaseTwoThree = _mm_shuffle_ps(vertices[1], vertices[2], _MM_SHUFFLE(1, 0, 1, 0));
		Math::Vector4 vertBaseThreeOne = _mm_shuffle_ps(vertices[2], vertices[0], _MM_SHUFFLE(1, 0, 1, 0));

		//Generating the dx/dy components for each of these vectors
		Math::Vector4 vert_dXdY_OneTwo = _mm_sub_ps (vertBaseTwoThree,vertBaseOneTwo);
		Math::Vector4 vert_dXdY_Three = _mm_sub_ps (vertBaseOneTwo,vertBaseThreeOne);

		//Swapping the dx/dy values
		vert_dXdY_OneTwo = _mm_shuffle_ps(vert_dXdY_OneTwo, vert_dXdY_OneTwo, _MM_SHUFFLE(2, 3, 0, 1));
		vert_dXdY_Three = _mm_shuffle_ps(vert_dXdY_Three, vert_dXdY_Three, _MM_SHUFFLE(2, 3, 0, 1));

		Math::Vector4 input, testEdgeOneTwo, testEdgeThree, finalTest;
		
		for(float y = (float) pixelBounds.minY; y <= (float)pixelBounds.maxY; ++y)
		{
			for(float x = (float)pixelBounds.minX; x <= (float)pixelBounds.maxX; ++x)
			{
				ALIGN float temp[4] = { x, y, x, y };
				input = Math::LoadVector4Aligned(temp);
				testEdgeOneTwo = _mm_sub_ps(input,vertBaseOneTwo);
				testEdgeThree = _mm_sub_ps(input,vertBaseThreeOne);

				testEdgeOneTwo = _mm_mul_ps(vert_dXdY_OneTwo,testEdgeOneTwo);
				testEdgeThree = _mm_mul_ps(vert_dXdY_Three,testEdgeThree);

				finalTest = _mm_hsub_ps(testEdgeOneTwo,testEdgeThree);
				
				if( ( ( _mm_movemask_ps( _mm_cmpgt_ps( finalTest, Math::zero )  ) & ( VERT_X | VERT_Y | VERT_Z) ) == ( VERT_X | VERT_Y | VERT_Z) ) && 
					( _mm_movemask_ps( _mm_cmpge_ps( finalTest, Math::zero )  ) & ( VERT_Z ) ) )
				{
					WriteBuffer(buffer,planeForInterpolation,(unsigned int)x,(unsigned int)y);
				}

			}
		}
	}

	void WriteBuffer(FramebufferBase* buffer, const Math::Vector4 &plane, const unsigned int &x, const unsigned int &y)
	{
		//in triangle or on line of one triangle
		Pixel<> pixel;
		float testX = 320.485;
		float testY = 239.83;
		//Interpolate z
		ALIGN float loadCoords[4] = { (float)testX, 1.0f,  (float)testY, 0.0f };
		Math::Vector4 coords = Math::LoadVector4Aligned(loadCoords);
		coords = _mm_mul_ps(coords,plane);
		coords = _mm_hadd_ps(coords,coords);
		coords = _mm_hadd_ps(coords,coords); //lowest order word has interpolated z value
		_mm_store_ss(&pixel.z, coords);
		pixel.red = 50;
		pixel.green = 50;
		pixel.blue = 50;
		
		buffer->PutPixel(&pixel,(unsigned int) x, (unsigned int) y);
	}


	void AddVertices(DataType* incVertices)
	{
		memcpy(vertices,incVertices,sizeof(DataType)*3);
		currentNumberVertices = 3;
	}

	void AddVertex(const DataType &vertex)
	{
		if(currentNumberVertices < 3)
			vertices[currentNumberVertices++] = vertex;
	}

private:
	DataType vertices[3];
	unsigned short currentNumberVertices;
	PipelineBase* pipeline;
};

#endif
