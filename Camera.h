
#ifndef CAMERA_H_
#define CAMERA_H_

#include "MatrixStack.h"
#include "MatrixMath.h"
#include "Vector.h"
#include <xmmintrin.h>
#include "Constants.h"
#include "Alignment.h"

static const Math::Vector4Constant cameraUp = { 0.0f, 1.0f, 0.0f, 1.0f };
static const Math::Vector4Constant cameraDefaultPosition = { -5.0f, 2.0f, 5.0f, 1.0f };

enum DirtyBits : unsigned short
{
	DIRTY_WORLD = 1,
	DIRTY_VIEW = 2,
	DIRTY_PROJECTION = 4,
	DIRTY_BOUNDS = 8
};

enum ProjectionType : unsigned short
{
	PERSPECTIVE = 1,
	ORTHOGRAPHIC = 2
};

enum CameraMatrices : unsigned short
{
	SCREEN_MATRIX = 0,
	PERSPECTIVE_MATRIX,
	PROJECTION_MATRIX,
	NUM_MATRICES
};

#define DEFAULT_FOV_DEGREES 35
#define DEFAULT_FOV DEFAULT_FOV_DEGREES*PI/180.0f
#define DEFAULT_NEAR_PLANE .01f
#define DEFAULT_FAR_PLANE 500.0f

using namespace Math;

ALIGN class Camera
{
public:
	Camera(	float incXDim = DEFAULT_WIDTHf,
			float incYDim = DEFAULT_HEIGHTf,
			float incFOV = DEFAULT_FOV,
			float incNearPlane = DEFAULT_NEAR_PLANE,
			float incFarPlane = DEFAULT_FAR_PLANE)	:	xDim(incXDim),
														yDim(incYDim),
														fov(incFOV),
														nearPlane(incNearPlane),
														farPlane(incFarPlane),
														up(cameraUp),
														lookAt(ident0),
														position(cameraDefaultPosition)

	{	
		/*up = Math::LoadVector4Aligned(cameraUpF);
		lookAt = Math::LoadVector4Aligned(ident0F);
		position = Math::LoadVector4Aligned(cameraDefaultPositionF);*/
		cameraStack = new MatrixStack<CameraMatrices,NUM_MATRICES>();
		SetupMatrices();
	}

	~Camera()
	{
		delete cameraStack;
	}

	ALIGNED_NEW

	const float& NearPlane()
	{
		return nearPlane;
	}

	const float& FarPlane()
	{
		return farPlane;
	}

	void SetupMatrices()
	{
		CameraMatrices toBeConstructed;
		while( ( toBeConstructed = ((MatrixStack<CameraMatrices,NUM_MATRICES>*)cameraStack)->TopType() ) != NUM_MATRICES)
		{
			switch(toBeConstructed)
			{
			case SCREEN_MATRIX:
				ComputeScreenMatrix();
				break;
			case PERSPECTIVE_MATRIX:
				ComputePerspectiveMatrix();
				break;
			case PROJECTION_MATRIX:
				ComputeProjectionMatrix();
				break;
			}
		}
	}

	const Matrix4X4& CameraMatrix()
	{
		if ( ((MatrixStack<CameraMatrices,NUM_MATRICES>*)cameraStack)->TopType()  != NUM_MATRICES )
			SetupMatrices();
		return ((MatrixStack<CameraMatrices,NUM_MATRICES>*)cameraStack)->Top();
	}

	const MatrixStackBase* CameraStack()
	{
		return cameraStack;
	}

	const Vector4& GetCameraLookAt() const
	{
		return lookAt;
	}
	void SetLookAt(Vector4 &incLookAt)
	{
		lookAt = incLookAt;
		SetCache(DIRTY_PROJECTION);
	}

	const Vector4& GetCameraUp() const
	{
		return up;
	}
	void SetCameraUp(Vector4 &incUp)
	{
		up = incUp;
		SetCache(DIRTY_PROJECTION);
	}

	const Vector4& GetCameraPosition() const
	{
		return position;
	}
	void SetCameraPosition(Vector4 &incPosition)
	{
		position = incPosition;
		SetCache(DIRTY_PROJECTION);
	}

	void TranslatePosition(Vector4 &translation)
	{
		SetCameraPosition(translation + position);
	}

	void ComputeScreenMatrix(bool replaceOnStack = false)
	{
		ALIGN float screenMatrix[4][4] = {	{ xDim / 2.0f,	0.0f,			0.0f,					xDim / 2.0f },
											{ 0.0f,			-yDim / 2.0f,	0.0f,					yDim / 2.0f },
											{ 0.0f,			0.0f,			tan( fov / 2 ),0.0f		},
											{ 0.0f,			0.0f,			0.0f,					1.0f		}};

		if(replaceOnStack)
			cameraStack->Pop();
		cameraStack->Push( Math::Matrix4X4( screenMatrix ) );
	}

	void ComputePerspectiveMatrix(bool replaceOnStack = false)
	{
		ALIGN float perspectiveCorrectionRow[4] = { 0.0f, 0.0f, tan( fov / 2 ), 1.0f };
		Vector4 castedRow = Math::LoadVector4Aligned( perspectiveCorrectionRow );
		//Generate matrix
		if(replaceOnStack)
			cameraStack->Pop();
		cameraStack->Push( Math::Matrix4X4( ident3, ident2, ident1, castedRow ) );
	}

	void ComputeProjectionMatrix(bool replaceOnStack = false)
	{
		//Calculate Z-Basis vector for camera projection matrix
		Vector4 zBasis = lookAt - position;
		Math::Normalize(zBasis);
		
		//Calculate Y-Basis vector for camera projection matrix
		Vector4 yBasis = up - ( Math::Vec3DotVec3(up,zBasis) ) * zBasis;
		Math::Normalize(yBasis);

		//Calculate X-Basis vector for camera projection matrix
		Vector4 xBasis = Math::Vec3CrossVec3(yBasis,zBasis);

		//Calculate "translation" - projection of world position on new basis
		//WTB sse equivalent of _mm_move_ss to pass through high word... god damnit
		Vector4 xBasisDotPos = Math::Vec3DotVec3(-xBasis,position);
		Vector4 yBasisDotPos = Math::Vec3DotVec3(-yBasis,position);
		Vector4 zBasisDotPos = Math::Vec3DotVec3(-zBasis,position);
		xBasisDotPos = _mm_move_ss(xBasis, xBasisDotPos);
		xBasis = _mm_shuffle_ps(xBasis, xBasisDotPos, _MM_SHUFFLE(0, 2, 1, 0));
		yBasisDotPos = _mm_move_ss(yBasis, yBasisDotPos);
		yBasis = _mm_shuffle_ps(yBasis, yBasisDotPos, _MM_SHUFFLE(0, 2, 1, 0));
		zBasisDotPos = _mm_move_ss(zBasis, zBasisDotPos);
		zBasis = _mm_shuffle_ps(zBasis, zBasisDotPos, _MM_SHUFFLE(0, 2, 1, 0));

		//Generate matrix
		if(replaceOnStack)
			cameraStack->Pop();
		cameraStack->Push( Math::Matrix4X4( xBasis, yBasis, zBasis, ident0 ) );
	}


private:
	void SetCache(DirtyBits bits)
	{
		dirtyCache |= bits;
	}

	bool isCacheDirty(DirtyBits isIt)
	{
		return (dirtyCache & isIt) == isIt;
	}
	void ResetCache()
	{
		dirtyCache = dirtyCache & DIRTY_BOUNDS; //masks everything but windows bounds check
	}

	float fov;
	float nearPlane;
	float farPlane;
	float xDim;
	float yDim;

	MatrixStackBase* cameraStack;
	Vector4 lookAt;
	Vector4 up;
	Vector4 position;

	unsigned short dirtyCache; // cache bitfield
};

#endif