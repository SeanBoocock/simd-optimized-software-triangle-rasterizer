#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef MATRIX_MATH_H_
#define MATRIX_MATH_H_

#include <xmmintrin.h>
#include "Constants.h"
#include "Vector.h"
#include <string.h>

#define _MM_MY_TRANSPOSE4_PS(row0, row1, row2, row3) {         \
            Math::Vector4 tmp2, tmp1, tmp0;                    \
                                                               \
            tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);     \
            tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);     \
            tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);     \
            (row0) = _mm_shuffle_ps(tmp0, tmp1, 0x88);         \
                                                               \
            (row1) = _mm_shuffle_ps(tmp0, tmp1, 0xDD);         \
            tmp0   = _mm_shuffle_ps((row2), (row3), 0xEE);     \
            (row2) = _mm_shuffle_ps(tmp2, tmp0, 0x88);         \
            (row3) = _mm_shuffle_ps(tmp2, tmp0, 0xDD);         \
        }

namespace Math
{
	typedef Vector4 Matrix4X4[4];
	
	extern void LoadMatrix_A(const float alignedMatrix[4][4], Matrix4X4 &outMat);

	extern void LoadMatrix(	const Vector4 &row0,
							const Vector4 &row1,
							const Vector4 &row2,
							const Vector4 &row3, 
							Matrix4X4 &outMat);

	extern void LoadMatrix_U(float* unalignedMatrix, Matrix4X4 &outMat);
	extern void LoadMatrix_U(float** unalignedMatrix, Matrix4X4 &outMat);

	//Some declarations
	extern void Matrix4X4Multiply(const Matrix4X4 &mat1, Matrix4X4 &mat2, Matrix4X4 &outMat, bool finalTranspose = true);
	extern void Transpose(Matrix4X4 &matrix);
	extern void MakeIdentity(Matrix4X4 &mat1);
	extern void BuildScale4x4Matrix(const float* inScale, Matrix4X4 &outMat);
	extern void BuildTranslation4x4Matrix(const float* inTranslation, Matrix4X4 &outMat);
	extern void BuildRotInX4x4Matrix(const float inRot, Matrix4X4 &outMat);
	extern void BuildRotInY4x4Matrix(const float inRot, Matrix4X4 &outMat);
	extern void BuildRotInZ4x4Matrix(const float inRot, Matrix4X4 &outMat);
	extern void BuildCombinedRot4x4Matrix(const float* inRot, Matrix4X4 &outMat);
}//namespace Math
#endif