
#include "MatrixMath.h"
#include <math.h>


namespace Math
{
	inline void LoadMatrix_A(const float alignedMatrix[4][4], Matrix4X4 &outMat)
	{
		outMat[0] = _mm_load_ps(alignedMatrix[0]);
		outMat[1] = _mm_load_ps(alignedMatrix[1]);
		outMat[2] = _mm_load_ps(alignedMatrix[2]);
		outMat[3] = _mm_load_ps(alignedMatrix[3]);
	}

	inline void LoadMatrix(	const Vector4 &row0,
							const Vector4 &row1,
							const Vector4 &row2,
							const Vector4 &row3, 
							Matrix4X4 &outMat	)
	{
		outMat[0] = row0;
		outMat[1] = row1;
		outMat[2] = row2;
		outMat[3] = row3;
	}

	inline void LoadMatrix_U(float* unalignedMatrix, Matrix4X4 &outMat)
	{
		outMat[0] = _mm_loadu_ps(&unalignedMatrix[0]);
		outMat[1] = _mm_loadu_ps(&unalignedMatrix[4]);
		outMat[2] = _mm_loadu_ps(&unalignedMatrix[8]);
		outMat[3] = _mm_loadu_ps(&unalignedMatrix[12]);
	}

	inline void LoadMatrix_U(float** unalignedMatrix, Matrix4X4 &outMat)
	{
		outMat[0] = _mm_loadu_ps(unalignedMatrix[0]);
		outMat[1] = _mm_loadu_ps(unalignedMatrix[1]);
		outMat[2] = _mm_loadu_ps(unalignedMatrix[2]);
		outMat[3] = _mm_loadu_ps(unalignedMatrix[3]);
	}

	inline void Matrix4X4Multiply(const Matrix4X4 &mat1, Matrix4X4 &mat2, Matrix4X4 &outMat, bool Transpose)
	{
		Vector4 m0,m1,m2,m3;

		if(Transpose)
			_MM_MY_TRANSPOSE4_PS(mat2[0], mat2[1], mat2[2], mat2[3]);
		//////////////////////////////////////////////
		m0 = _mm_dp_ps(mat1[0], mat2[0],255);
		m1 = _mm_dp_ps(mat1[0], mat2[1],255);
		m2 = _mm_dp_ps(mat1[0], mat2[2],255);
		m3 = _mm_dp_ps(mat1[0], mat2[3],255);
		outMat[0] = _mm_shuffle_ps(m0,m1,_MM_SHUFFLE(1, 1, 0, 0));
		outMat[0] = _mm_shuffle_ps(outMat[0],m2,_MM_SHUFFLE(1, 1, 2, 0));
		outMat[0] = _mm_insert_ps(outMat[0],m3,_MM_MK_INSERTPS_NDX(3,3,0));
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_dp_ps(mat1[1], mat2[0],255);
		m1 = _mm_dp_ps(mat1[1], mat2[1],255);
		m2 = _mm_dp_ps(mat1[1], mat2[2],255);
		m3 = _mm_dp_ps(mat1[1], mat2[3],255);
		outMat[1] = _mm_shuffle_ps(m0,m1,_MM_SHUFFLE(1, 1, 0, 0));
		outMat[1] = _mm_shuffle_ps(outMat[1],m2,_MM_SHUFFLE(1, 1, 2, 0));
		outMat[1] = _mm_insert_ps(outMat[1],m3,_MM_MK_INSERTPS_NDX(3,3,0));
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_dp_ps(mat1[2], mat2[0],255);
		m1 = _mm_dp_ps(mat1[2], mat2[1],255);
		m2 = _mm_dp_ps(mat1[2], mat2[2],255);
		m3 = _mm_dp_ps(mat1[2], mat2[3],255);
		outMat[2] = _mm_shuffle_ps(m0,m1,_MM_SHUFFLE(1, 1, 0, 0));
		outMat[2] = _mm_shuffle_ps(outMat[2],m2,_MM_SHUFFLE(1, 1, 2, 0));
		outMat[2] = _mm_insert_ps(outMat[2],m3,_MM_MK_INSERTPS_NDX(3,3,0));
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_dp_ps(mat1[3], mat2[0],255);
		m1 = _mm_dp_ps(mat1[3], mat2[1],255);
		m2 = _mm_dp_ps(mat1[3], mat2[2],255);
		m3 = _mm_dp_ps(mat1[3], mat2[3],255);
		outMat[3] = _mm_shuffle_ps(m0,m1,_MM_SHUFFLE(1, 1, 0, 0));
		outMat[3] = _mm_shuffle_ps(outMat[3],m2,_MM_SHUFFLE(1, 1, 2, 0));
		outMat[3] = _mm_insert_ps(outMat[3],m3,_MM_MK_INSERTPS_NDX(3,3,0));
		//////////////////////////////////////////////
		if(Transpose)
			_MM_MY_TRANSPOSE4_PS(mat2[0], mat2[1], mat2[2], mat2[3]);
	}

	//inline Matrix4X4 Matrix4X4Multiply(const Matrix4X4 &mat1, Matrix4X4 &mat2, bool Transpose)
	//{
	//	Matrix4X4 toReturn;

	//	Vector4 m0,m1,m2,m3,m4,m5;

	//	if(Transpose)
	//		_MM_MY_TRANSPOSE4_PS(mat2[0], mat2[1], mat2[2], mat2[3]);
	//	//////////////////////////////////////////////
	//	m0 = _mm_mul_ps(mat1[0], mat2[0]);
	//	m1 = _mm_mul_ps(mat1[0], mat2[1]);
	//	m2 = _mm_mul_ps(mat1[0], mat2[2]);
	//	m3 = _mm_mul_ps(mat1[0], mat2[3]);
	//	_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
	//	m4 = _mm_add_ps(m0,m1);
	//	m5 = _mm_add_ps(m2,m3);
	//	toReturn[0] = _mm_add_ps(m4,m5);
	//	//////////////////////////////////////////////

	//	//////////////////////////////////////////////
	//	m0 = _mm_mul_ps(mat1[1], mat2[0]);
	//	m1 = _mm_mul_ps(mat1[1], mat2[1]);
	//	m2 = _mm_mul_ps(mat1[1], mat2[2]);
	//	m3 = _mm_mul_ps(mat1[1], mat2[3]);
	//	_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
	//	m4 = _mm_add_ps(m0,m1);
	//	m5 = _mm_add_ps(m2,m3);
	//	toReturn[1] = _mm_add_ps(m4,m5);
	//	//////////////////////////////////////////////

	//	//////////////////////////////////////////////
	//	m0 = _mm_mul_ps(mat1[2], mat2[0]);
	//	m1 = _mm_mul_ps(mat1[2], mat2[1]);
	//	m2 = _mm_mul_ps(mat1[2], mat2[2]);
	//	m3 = _mm_mul_ps(mat1[2], mat2[3]);
	//	_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
	//	m4 = _mm_add_ps(m0,m1);
	//	m5 = _mm_add_ps(m2,m3);
	//	toReturn[2] = _mm_add_ps(m4,m5);
	//	//////////////////////////////////////////////

	//	//////////////////////////////////////////////
	//	m0 = _mm_mul_ps(mat1[3], mat2[0]);
	//	m1 = _mm_mul_ps(mat1[3], mat2[1]);
	//	m2 = _mm_mul_ps(mat1[3], mat2[2]);
	//	m3 = _mm_mul_ps(mat1[3], mat2[3]);
	//	_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
	//	m4 = _mm_add_ps(m0,m1);
	//	m5 = _mm_add_ps(m2,m3);
	//	toReturn[3] = _mm_add_ps(m4,m5);
	//	//////////////////////////////////////////////
	//	if(Transpose)
	//		_MM_MY_TRANSPOSE4_PS(mat2[0], mat2[1], mat2[2], mat2[3]);
	//	return toReturn;
	//}

	inline void Transpose(Matrix4X4 &matrix)
	{
		_MM_MY_TRANSPOSE4_PS(matrix[0], matrix[1], matrix[2], matrix[3]);
	}

	inline void MakeIdentity(Matrix4X4 &mat1)
	{
		ALIGN float ident[4][4] = {	{ 1.0f, 0.0f, 0.0f, 0.0f },
									{ 0.0f, 1.0f, 0.0f, 0.0f },
									{ 0.0f, 0.0f, 1.0f, 0.0f },
									{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		LoadMatrix_A(ident,mat1);
	}

	inline void BuildScale4x4Matrix(const float* inScale, Matrix4X4 &outMat)
	{
		ALIGN float scale[4][4] = {	{ inScale[0], 0.0f, 0.0f, 0.0f },
													{ 0.0f, inScale[1], 0.0f, 0.0f },
													{ 0.0f, 0.0f, inScale[2], 0.0f },
													{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		LoadMatrix_A(scale,outMat);
	}

	inline void BuildTranslation4x4Matrix(const float* inTranslation, Matrix4X4 &outMat)
	{
		ALIGN float trans[4][4] = {	{ 1.0f, 0.0f, 0.0f, inTranslation[0] },
									{ 0.0f, 1.0f, 0.0f, inTranslation[1] },
									{ 0.0f, 0.0f, 1.0f, inTranslation[2] },
									{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		LoadMatrix_A(trans,outMat);
	}

	inline void BuildRotInX4x4Matrix(const float inRot, Matrix4X4 &outMat)
	{
		// 1		0		0		0
		// 0		cos()	-sin()	0
		// 0		sin()	cos()	0
		// 0		0		0		1
		ALIGN float rot[4][4] = {	{ 1.0f,		0.0f,		0.0f,			0.0f },
									{ 0.0f,		cos(inRot), -sin(inRot),	0.0f },
									{ 0.0f,		sin(inRot),	cos(inRot),		0.0f },
									{ 0.0f,		0.0f,		0.0f,			1.0f }	};
		LoadMatrix_A(rot,outMat);
	}

	inline void BuildRotInY4x4Matrix(const float inRot, Matrix4X4 &outMat)
	{
		// cos()	0		sin()	0
		// 0		1		0		0
		// -sin()	0		cos()	0
		// 0		0		0		1
		ALIGN float rot[4][4] = {	{ cos(inRot),	0.0f,	sin(inRot),	0.0f },
									{ 0.0f,			1.0f,	0.0f,		0.0f },
									{ -sin(inRot),	0.0f,	cos(inRot),	0.0f },
									{ 0.0f,			0.0f,	0.0f,		1.0f }	};
		LoadMatrix_A(rot,outMat);
	}

	inline void BuildRotInZ4x4Matrix(const float inRot, Matrix4X4 &outMat)
	{
		// cos()	-sin()	0		1
		// sin()	cos()	0		1
		// 0		0		1		1
		// 0		0		0		1
		ALIGN float rot[4][4] = {	{ cos(inRot),	-sin(inRot),	0.0f,	0.0f },
									{ sin(inRot),	cos(inRot),		0.0f,	0.0f },
									{ 0.0f,			0.0f,			1.0f,	0.0f },
									{ 0.0f,			0.0f,			0.0f,	1.0f }	};
		LoadMatrix_A(rot,outMat);
	}

	inline void BuildCombinedRot4x4Matrix(const float* inRot, Matrix4X4 &outMat)
	{

	}

}