
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Primitive.h"
//#include "Texture.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/tbb.h"

class ExecutePrimitives
{
	VertexBuffer * const buffer;
public:
	void operator () (const tbb::blocked_range<size_t>& r) const
	{
		VertexBuffer* localBuffer = buffer;
		for(size_t i = r.begin(); i != r.end(); ++i)
			localBuffer->At(i)->ExecutePipeline();
	}
	ExecutePrimitives( VertexBuffer* buf) : buffer(buf)
	{}
};

Renderer::Renderer()	:	camera(nullptr),
							numTargets(2),
							currentTarget(0),
							numGBuffers(1),
							shadeMode(PHONG)
{
	
	targets = new RenderTarget[numTargets];
	camera = new Camera();

	renderStack = new MatrixStack<RenderMatrices,1>();
	renderStack->SetStackBelow(camera->CameraStack());

	//TEMPORARY-SHADER/LIGHTS
	ALIGN float lightTemp[4] = {-0.7071f, 0.7071f, 0.0f, 0.0f};
	lights[0].lightDir = Math::LoadVector4Aligned(lightTemp);//directions
	ALIGN float lightTemp1[4] = {0.5f, 0.5f, 0.9f, 0.0f};
	lights[0].lightColor = Math::LoadVector4Aligned(lightTemp1);
	ALIGN float lightTemp2[4] = {0.0f, -0.7071f, -0.7071f, 0.0f};
	lights[1].lightDir = Math::LoadVector4Aligned(lightTemp2);//directions
	ALIGN float lightTemp3[4] = {0.9f, 0.2f, 0.3f, 0.0f};
	lights[1].lightColor = Math::LoadVector4Aligned(lightTemp3);//directions
	ALIGN float lightTemp4[4] = {0.7071f, 0.0f, -0.7071f, 0.0f};
	lights[2].lightDir = Math::LoadVector4Aligned(lightTemp4);//directions
	ALIGN float lightTemp5[4] = {0.2f, 0.7f, 0.3f, 0.0f};
	lights[2].lightColor = Math::LoadVector4Aligned(lightTemp5);//directions

	ambientLight.lightDir = Math::zero; //color value
	ALIGN float lightTemp6[4] = {0.3f, 0.3f, 0.3f, 0.0f};
	ambientLight.lightColor = Math::LoadVector4Aligned(lightTemp4);

	//power is 32, taking log2(power)
	specularPower = (int) log(32.0f)/log(2.0f); // log base converion to base 2

	ALIGN float specCoef[4] = {0.3f, 0.3f, 0.3f, 0.0f};
	specCoefficient = Math::LoadVector4Aligned(specCoef);
	ALIGN float diffCoef[4] = {0.7f, 0.7f, 0.7f, 0.0f};
	diffuseCoefficient = Math::LoadVector4Aligned(diffCoef);
	ALIGN float ambCoef[4] = {0.1f, 0.1f, 0.1f, 0.0f};
	ambientCoefficient = Math::LoadVector4Aligned(ambCoef);

	//This will double as z-buffer/interpolated normal for now
	/*gBuffers = (FramebufferBase**) new FrameBuffer<Pixel<Math::Vector4,Depth,1>, Math::Vector4, Depth>*[numGBuffers];
	for(int i = 0; i < numGBuffers; ++i)
		gBuffers[i] = new FrameBuffer<Pixel<Math::Vector4,Depth,1>, Math::Vector4, Depth>();*/
}

Renderer::~Renderer()
{
	/*for(unsigned int i = 0; i < numTargets; ++i)
		delete targets[i];*/
	delete [] targets;
	delete camera;
	/*if(gBuffers)	delete [] gBuffers;*/
}

HRESULT Renderer::Initialize()
{
	camera->SetupMatrices();
	return S_OK;
}

RenderTarget* Renderer::Draw()
{
	unsigned int result = RENDERER_SUCCESS;
	
	currentTarget = (++currentTarget < numTargets) ? currentTarget : 0;
	targets[currentTarget].GetBuffer()->Reset();
	gBuffers[0].Reset();

	//Setup matrix stack for this mesh
	result |= InitializeMatricesForMesh();
	if( ( result & RENDERER_FATAL_ERROR ) == RENDERER_FATAL_ERROR )
	{
		DEBUG_PRINT("Failure of initializing matrices.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	result |= InitializeClippingLimits();
	if( ( result & RENDERER_FATAL_ERROR ) == RENDERER_FATAL_ERROR )
	{
		DEBUG_PRINT("Failure of initializing clipping limits.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}


	/*for(unsigned int primitive = 0; primitive < vertexBuffer->Size(); ++primitive)
	{
		vertexBuffer->At(primitive)->ExecutePipeline();
	}*/

	tbb::parallel_for(tbb::blocked_range<size_t>(0, vertexBuffer->Size(), 50), ExecutePrimitives(vertexBuffer), tbb::simple_partitioner() );

	/*tbb::parallel_for(0,iterations,[&](int primitive)
	{
		vertexBuffer->At(primitive)->ExecutePipeline();
	});*/

	//after all meshes drawn, do shading for all pixels drawn in depth buffer
	result |= Shader();
	if( ( result & RENDERER_FATAL_ERROR ) == RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	renderStack->Pop();

	return &targets[currentTarget];
}

unsigned int Renderer::InitializeClippingLimits()
{
	/*ALIGN float min[4] = { 0.0f, 0.0f, camera->NearPlane(), 0.0f };*/
	clipMin = Math::zero;
	ALIGN float max[4] = { targets[currentTarget].GetWidth(), targets[currentTarget].GetHeight(), camera->FarPlane(), 1.0f };
	clipMax = Math::LoadVector4Aligned( max );
	return RENDERER_SUCCESS;
}

void Renderer::SetCamera(Camera* incCamera)
{
	camera = (incCamera) ? incCamera : camera;
}

void Renderer::SubmitVertexBuffer(VertexBuffer* buf)
{
	vertexBuffer = (buf) ? buf : nullptr;
}

unsigned int Renderer::InitializeMatricesForMesh()
{
	DEBUG_PRINT("Renderer executing InitializeMatricesForMesh().\n");
	static float  rotation = 0.01f;
	rotation += .01f;
	Math::Matrix4X4 rot;
	//Math::MakeIdentity(rot);
	Math::BuildRotInY4x4Matrix(rotation,rot);
	renderStack->Push(rot);
	return RENDERER_SUCCESS;
}

unsigned int Renderer::TransformVertices(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing TransformVertices(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());
	primitive->TransformVertices( renderStack->Top() );
	return RENDERER_SUCCESS;
}

unsigned int Renderer::ClipPrimitives(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing ClipPrimitives(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());	
	return primitive->ClipVertices(Math::zero,clipMax);
}

unsigned int Renderer::SortVertices(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing SortVertices(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());
	//Clockwise edge sorting.  Convention is that after this transformation, vertices in the primitive will be sorted in a clockwise fashion.
	primitive->SortVertices();
	return RENDERER_SUCCESS;
}

unsigned int Renderer::GenerateBounds(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing GenerateBounds(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());
	/*called during SortVertices in current implementation*/
	return RENDERER_SUCCESS;
}

unsigned int Renderer::LEETest(PrimitiveBase* primitive)
{
	//for all pixels in bounding box of primitive
	//if pass test
	//interpolate z at x,y coordinate
	//
	DEBUG_PRINT("Renderer executing LEETest(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());
	static_cast<Primitive<>*>(primitive)->LEETest(gBuffers[0]);
	//WriteGBuffer();
	return RENDERER_SUCCESS;
}

unsigned int Renderer::WriteGBuffer()
{
	DEBUG_PRINT("Renderer executing WriteGBuffer().\n");
	return RENDERER_SUCCESS;
}

Math::Vector4 Renderer::CalculateSpecPower(const Math::Vector4& toPower)
{
	Math::Vector4 toReturn = toPower;
	for(int i = 0; i < specularPower; ++i)
		toReturn = _mm_mul_ps(toReturn,toPower);
	return toReturn;
}

unsigned int Renderer::Shader()
{
	DEBUG_PRINT("Renderer executing Shader().\n");
	/*FrameBuffer<Pixel<Math::Vector4,Depth,1>, Math::Vector4, Depth>* localBuf = static_cast<FrameBuffer<Pixel<Math::Vector4,Depth,1>, Math::Vector4, Depth>*>(gBuffers[0]);
*/
	

	unsigned int xDim = (unsigned int)gBuffers[0].GetWidth();
	unsigned int yDim = (unsigned int)gBuffers[0].GetHeight();

	Math::Vector4 texture = Math::ident;
	
	/*for(unsigned int val = 0; val < xDim*yDim; ++val)*/
	tbb::parallel_for(0,(int)(xDim*yDim),[=](int val)
	{
		if(gBuffers[0].GetPixel(val).IsWrittenTo()) //Pixel was updated during last frame
		{
			Pixel<Math::Vector4,Depth,1> currentNormal;
			unsigned short skipped = 0;
			Math::Vector4 nDotL,nDotE,origNormal,normal,reflection,rDotE,computedColor,temp,temp2;
			currentNormal = gBuffers[0].GetPixel(val);
			origNormal = currentNormal.data[0];
			computedColor = Math::zero;

			for(int i = 0; i < numLights; ++i) // 2 = num lights for now
			{
				bool compare1,compare2;
				normal = origNormal;
				nDotL = Math::Vec3DotVec3(lights[i].lightDir,normal);
				nDotE = _mm_mul_ps(negate,normal);
				nDotE = _mm_shuffle_ps(nDotE, nDotE, _MM_SHUFFLE(2, 2, 2, 2));
				compare1 = ( _mm_movemask_ps( _mm_cmpge_ps( nDotL, Math::zero )  ) & VERT_Z ) == VERT_Z;
				compare2 = ( _mm_movemask_ps( _mm_cmplt_ps( nDotE, Math::zero )  ) & VERT_Z ) == VERT_Z;
				if(compare1 ^ compare2) 
				{
					//light is on opposite side of the surface, thus different signs (xor == 0) thus we skip
					++skipped;
					continue;
				}
				if( compare1 == false && compare2 == true ) //both values were negative
				{
					nDotL = _mm_mul_ps(negate,nDotL);
					nDotE = _mm_mul_ps(negate,nDotE);
					normal = _mm_mul_ps(negate,normal);
				}

				reflection = _mm_sub_ps( _mm_mul_ps( _mm_mul_ps( Math::twice, nDotL ), normal ), lights[i].lightDir);
				Math::Normalize(reflection);
				rDotE = _mm_sub_ps( Math::zero ,  reflection); // want to flip the z value
				if( (_mm_movemask_ps( _mm_cmplt_ps( rDotE, Math::zero ) ) & VERT_Z ) == VERT_Z )
					rDotE = Math::zero;
				else
					rDotE = _mm_shuffle_ps(rDotE, rDotE, _MM_SHUFFLE(2, 2, 2, 2)); //duplicate z value accross all words

				if(shadeMode == PHONG)
				{
					//Specular calc
					temp = _mm_mul_ps(lights[i].lightColor,CalculateSpecPower(rDotE));
					temp = _mm_mul_ps(temp,specCoefficient);
					//Diffuse calc
					temp2 = _mm_mul_ps(diffuseCoefficient,lights[i].lightColor);
					temp2 = _mm_mul_ps(temp2,nDotL);
					//Combine terms
					temp = _mm_add_ps(temp,temp2);
					computedColor = _mm_add_ps(computedColor, temp);
				}
				else if (shadeMode == GOURAUD)
				{
					computedColor = _mm_add_ps(computedColor, _mm_add_ps( _mm_mul_ps(lights[i].lightColor, CalculateSpecPower(rDotE) ), _mm_mul_ps(diffuseCoefficient, _mm_mul_ps( lights[i].lightColor, nDotL ) ) ) );
					computedColor = _mm_add_ps( computedColor, ambientLight.lightColor );
				}
			} // for lights
			if(skipped <= numLights)
			{
				//Calculate ambient term and add it
				computedColor = _mm_add_ps( computedColor, _mm_mul_ps( ambientCoefficient, ambientLight.lightColor ) );

				//With color computed, get ready to store it
				//TESTING JUST WITH NORMALS
				/*computedColor = _mm_mul_ps(colorScale,origNormal);*/
				computedColor = _mm_mul_ps(colorScale,computedColor);
				computedColor = _mm_mul_ps(negate,computedColor);
				Pixel<> pixel;
				ALIGN float tempColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f};
				_mm_store_ps(tempColor,computedColor);
				for(int i = 0; i < 3; ++i)
					pixel.color[i] = (tempColor[i] < 0) ? 0 : ( tempColor[i] > 255 ? 255 : (Intensity)((long)tempColor[i]) );
				targets[currentTarget].GetBuffer()->PutPixel(std::forward<Pixel<>>(pixel),val,true);
			}
			/*else
			{
				Pixel<>* pixel = new Pixel<>();
				unsigned short TESTING[4] = { 255, 0, 0, 0 };
				for(int i = 0; i < 3; ++i)
					pixel->color[i]=TESTING[i];
				targets[currentTarget]->GetBuffer()->PutPixel(pixel,val,true);
			}*/
		}// if(localBuf->GetPixel(val)->IsWrittenTo()
	});// for(unsigned int val = 0; val < xDim*yDim; ++val)
	
	return RENDERER_SUCCESS;
}