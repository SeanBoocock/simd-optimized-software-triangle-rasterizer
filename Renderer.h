
#ifndef RENDERER_H_
#define RENDERER_H_

#include <Windows.h>
#include "Singleton.h"
#include "Vector.h"
#include "Alignment.h"

class Camera;
class Buffer;
class FramebufferBase;
class RenderTarget;
class PrimitiveBase;

#define RENDERER Renderer::GetInstancePtr()

ALIGN class Renderer	: public Singleton<Renderer>
{
public:
	friend class Singleton<Renderer>;

	ALIGNED_NEW

	HRESULT Initialize();

	RenderTarget* Draw();

	void SetCamera(Camera* incCamera);

	void SubmitVertexBuffer(Buffer* buf);

	unsigned int TransformVertices(PrimitiveBase* primitive);

	unsigned int ClipPrimitives(PrimitiveBase* primitive);

	unsigned int SortVertices(PrimitiveBase* primitive);

	unsigned int GenerateBounds(PrimitiveBase* primitive);

	unsigned int LEETest(PrimitiveBase* primitive);

private:
	
	Renderer();
	
	~Renderer();

	unsigned int InitializeMatricesForMesh();

	unsigned int InitializeClippingLimits();

	unsigned int WriteGBuffer();

	unsigned int Shader();

	Camera* camera;
	Buffer* vertexBuffer;

	//clipping to view frustrum in screen space
	Math::Vector4 clipMin;
	Math::Vector4 clipMax;

	unsigned short numTargets;
	unsigned short currentTarget;
	RenderTarget** targets; //back and front

	unsigned short numGBuffers;
	FramebufferBase** gBuffers; //intermediate buffers before final shading
};



#endif