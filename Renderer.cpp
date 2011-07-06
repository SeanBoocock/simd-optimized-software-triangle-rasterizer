
#include "Renderer.h"
#include "RenderTarget.h"
#include "ErrorHandling.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Primitive.h"

Renderer::Renderer()	:	camera(nullptr),
							vertexBuffer(nullptr),
							numTargets(2),
							currentTarget(0),
							numGBuffers(1)
{
	
	targets = new RenderTarget*[numTargets];
	for(unsigned int i = 0; i < numTargets; ++i)
		targets[i] = new RenderTarget();
camera = new Camera();
}

Renderer::~Renderer()
{
	for(unsigned int i = 0; i < numTargets; ++i)
		delete targets[i];
	delete targets;
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

	//Setup matrix stack for this mesh
	result |= InitializeMatricesForMesh();
	if( result & RENDERER_FATAL_ERROR )
	{
		DEBUG_PRINT("Failure of initializing matrices.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	result |= InitializeClippingLimits();
	if( result & RENDERER_FATAL_ERROR )
	{
		DEBUG_PRINT("Failure of initializing clipping limits.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	for(unsigned int primitive = 0; primitive < vertexBuffer->Size(); ++primitive)
	{
		vertexBuffer->At(primitive)->ExecutePipeline();
	}

#if 0
	//Batch transform vertices for this mesh based on stack
	result |= TransformVertices(primitive);
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	//Clip near/far plane
	result |= ClipPrimitives(primitive);
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	//Sort vertices top to bottom, left to right
	result |= SortVertices(primitive);
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	//Generate x-y bounding box of pixels
	result |= GenerateBounds(primitive);
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	//For all pixels in bounding box do LEE test to determine if they're in the triangle (handle special cases on lines/vertices)
	result |= LEETest(primitive);
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	//Interpolate z for all pixels inside triangle and write z values to depth buffer if they are smaller than existing depth buffer values
	result |= WriteGBuffer();
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}
#endif

	//after all meshes drawn, do shading for all pixels drawn in depth buffer
	result |= Shader();
	if( result & RENDERER_FATAL_ERROR )
	{
		printf("General failure.  Aborting.  Flags: %d\n",result);
		return nullptr;
	}

	return targets[currentTarget];
}

unsigned int Renderer::InitializeClippingLimits()
{
	ALIGN float min[4] = { 0.0f, 0.0f, camera->NearPlane(), 0.0f };
	clipMin = Math::LoadVector4Aligned( min );
	ALIGN float max[4] = { targets[currentTarget]->GetWidth(), targets[currentTarget]->GetHeight(), camera->FarPlane(), 1.0f };
	clipMax = Math::LoadVector4Aligned( max );
	return RENDERER_SUCCESS;
}

void Renderer::SetCamera(Camera* incCamera)
{
	camera = (incCamera) ? incCamera : camera;
}

void Renderer::SubmitVertexBuffer(Buffer* buf)
{
	vertexBuffer = (buf) ? buf : vertexBuffer;
}

unsigned int Renderer::InitializeMatricesForMesh()
{
	DEBUG_PRINT("Renderer executing InitializeMatricesForMesh().\n");
	return RENDERER_SUCCESS;
}

unsigned int Renderer::TransformVertices(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing TransformVertices(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());
	primitive->TransformVertices( camera->CameraMatrix() );
	return RENDERER_SUCCESS;
}

unsigned int Renderer::ClipPrimitives(PrimitiveBase* primitive)
{
	DEBUG_PRINT("Renderer executing ClipPrimitives(PrimitiveBase* primitive) for primitive: %d.\n", primitive->ID());	
	return primitive->ClipVertices(clipMin,clipMax);
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
	primitive->LEETest(targets[currentTarget]->GetBuffer());
	WriteGBuffer();
	return RENDERER_SUCCESS;
}

unsigned int Renderer::WriteGBuffer()
{
	DEBUG_PRINT("Renderer executing WriteGBuffer().\n");
	return RENDERER_SUCCESS;
}

unsigned int Renderer::Shader()
{
	DEBUG_PRINT("Renderer executing Shader().\n");
	return RENDERER_SUCCESS;
}