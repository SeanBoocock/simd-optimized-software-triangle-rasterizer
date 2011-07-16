
#ifndef RENDERER_H_
#define RENDERER_H_

#include <Windows.h>
#include "Singleton.h"
#include "Vector.h"
#include "Alignment.h"
#include "FrameBuffer.h"
#include "PrimitiveBase.h"
#include "RenderTarget.h"
#include "ErrorHandling.h"
#include "Camera.h"

#include "MatrixStack.h"


class VertexBuffer;
//class MatrixStackBase;
//class Camera;
//class RenderTarget;

#define numLights 3
#define NUMBER_G_BUFFERS 1

enum ShaderMode	: unsigned short
{
	PHONG,
	GOURAUD
};

enum RenderMatrices
{
	ROTATION
};

#define RENDERER Renderer::GetInstancePtr()

ALIGN struct Light
{
	ALIGN Math::Vector4 lightDir;
	ALIGN Math::Vector4 lightColor;
};

ALIGN class Renderer	: public Singleton<Renderer>
{
public:
	friend class Singleton<Renderer>;

	ALIGNED_NEW

	HRESULT Initialize();

	RenderTarget* Draw();

	void SetCamera(Camera* incCamera);

	void SubmitVertexBuffer(VertexBuffer* buf);

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
	MatrixStackBase* renderStack;
	VertexBuffer* vertexBuffer;

	//clipping to view frustrum in screen space
	Math::Vector4 clipMin;
	Math::Vector4 clipMax;

	unsigned short numTargets;
	unsigned short currentTarget;
	RenderTarget* targets; //back and front

	unsigned short numGBuffers;
	FrameBuffer< Pixel<Math::Vector4,Depth,1>, Math::Vector4, Depth > gBuffers[NUMBER_G_BUFFERS]; //intermediate buffers before final shading

	//TEMPORARY LIGHTS/Shader constants
	Light lights[numLights];//directions
	Light ambientLight; //color value
	int specularPower;
	Math::Vector4 CalculateSpecPower(const Math::Vector4& toPower);
	Math::Vector4 specCoefficient;
	Math::Vector4 diffuseCoefficient;
	Math::Vector4 ambientCoefficient;
	ShaderMode shadeMode;
};



#endif
