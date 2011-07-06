
#include "MatrixMath.h"
#include <math.h>


namespace Math
{
	Matrix4X4::Matrix4X4()
	{

	}

	Matrix4X4::Matrix4X4(const float alignedMatrix[4][4])
	{
		Load(alignedMatrix);
	}

	Matrix4X4::Matrix4X4(	const Vector4 &row0,
				const Vector4 &row1,
				const Vector4 &row2,
				const Vector4 &row3	)
	{
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
		rows[3] = row3;
	}

	void Matrix4X4::Load(float* unalignedMatrix)
	{
		rows[0] = _mm_loadu_ps(&unalignedMatrix[0]);
		rows[1] = _mm_loadu_ps(&unalignedMatrix[4]);
		rows[2] = _mm_loadu_ps(&unalignedMatrix[8]);
		rows[3] = _mm_loadu_ps(&unalignedMatrix[12]);
	}

	void Matrix4X4::Load(float** unalignedMatrix)
	{
		rows[0] = _mm_loadu_ps(unalignedMatrix[0]);
		rows[1] = _mm_loadu_ps(unalignedMatrix[1]);
		rows[2] = _mm_loadu_ps(unalignedMatrix[2]);
		rows[3] = _mm_loadu_ps(unalignedMatrix[3]);
	}

	void Matrix4X4::Load(const float alignedMatrix[4][4])
	{
		rows[0] = _mm_load_ps(alignedMatrix[0]);
		rows[1] = _mm_load_ps(alignedMatrix[1]);
		rows[2] = _mm_load_ps(alignedMatrix[2]);
		rows[3] = _mm_load_ps(alignedMatrix[3]);
	}

	inline Matrix4X4 Matrix4X4Multiply(const Matrix4X4 &mat1, Matrix4X4 &mat2, bool Transpose)
	{
		Matrix4X4 toReturn;

		Vector4 m0,m1,m2,m3,m4,m5;

		if(Transpose)
			_MM_MY_TRANSPOSE4_PS(mat2.rows[0], mat2.rows[1], mat2.rows[2], mat2.rows[3]);
		//////////////////////////////////////////////
		m0 = _mm_mul_ps(mat1.rows[0], mat2.rows[0]);
		m1 = _mm_mul_ps(mat1.rows[0], mat2.rows[1]);
		m2 = _mm_mul_ps(mat1.rows[0], mat2.rows[2]);
		m3 = _mm_mul_ps(mat1.rows[0], mat2.rows[3]);
		_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
		m4 = _mm_add_ps(m0,m1);
		m5 = _mm_add_ps(m2,m3);
		toReturn.rows[0] = _mm_add_ps(m4,m5);
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_mul_ps(mat1.rows[1], mat2.rows[0]);
		m1 = _mm_mul_ps(mat1.rows[1], mat2.rows[1]);
		m2 = _mm_mul_ps(mat1.rows[1], mat2.rows[2]);
		m3 = _mm_mul_ps(mat1.rows[1], mat2.rows[3]);
		_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
		m4 = _mm_add_ps(m0,m1);
		m5 = _mm_add_ps(m2,m3);
		toReturn.rows[1] = _mm_add_ps(m4,m5);
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_mul_ps(mat1.rows[2], mat2.rows[0]);
		m1 = _mm_mul_ps(mat1.rows[2], mat2.rows[1]);
		m2 = _mm_mul_ps(mat1.rows[2], mat2.rows[2]);
		m3 = _mm_mul_ps(mat1.rows[2], mat2.rows[3]);
		_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
		m4 = _mm_add_ps(m0,m1);
		m5 = _mm_add_ps(m2,m3);
		toReturn.rows[2] = _mm_add_ps(m4,m5);
		//////////////////////////////////////////////

		//////////////////////////////////////////////
		m0 = _mm_mul_ps(mat1.rows[3], mat2.rows[0]);
		m1 = _mm_mul_ps(mat1.rows[3], mat2.rows[1]);
		m2 = _mm_mul_ps(mat1.rows[3], mat2.rows[2]);
		m3 = _mm_mul_ps(mat1.rows[3], mat2.rows[3]);
		_MM_TRANSPOSE4_PS(m0, m1, m2, m3);
		m4 = _mm_add_ps(m0,m1);
		m5 = _mm_add_ps(m2,m3);
		toReturn.rows[3] = _mm_add_ps(m4,m5);
		//////////////////////////////////////////////
		if(Transpose)
			_MM_MY_TRANSPOSE4_PS(mat2.rows[0], mat2.rows[1], mat2.rows[2], mat2.rows[3]);
		return toReturn;
	}

	inline void Transpose(Matrix4X4 &matrix)
	{
		_MM_MY_TRANSPOSE4_PS(matrix.rows[0], matrix.rows[1], matrix.rows[2], matrix.rows[3]);
	}

	inline void MakeIdentity(Matrix4X4 &mat1)
	{
		ALIGN float ident[4][4] = {	{ 1.0f, 0.0f, 0.0f, 0.0f },
													{ 0.0f, 1.0f, 0.0f, 0.0f },
													{ 0.0f, 0.0f, 1.0f, 0.0f },
													{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		mat1.Load(ident);
	}

	inline void BuildScale4x4Matrix(const float* inScale, Matrix4X4 &outMat)
	{
		ALIGN float scale[4][4] = {	{ inScale[0], 0.0f, 0.0f, 0.0f },
													{ 0.0f, inScale[1], 0.0f, 0.0f },
													{ 0.0f, 0.0f, inScale[2], 0.0f },
													{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		outMat.Load(scale);
	}

	inline void BuildTranslation4x4Matrix(const float* inTranslation, Matrix4X4 &outMat)
	{
		ALIGN float trans[4][4] = {	{ 1.0f, 0.0f, 0.0f, inTranslation[0] },
													{ 0.0f, 1.0f, 0.0f, inTranslation[1] },
													{ 0.0f, 0.0f, 1.0f, inTranslation[2] },
													{ 0.0f, 0.0f, 0.0f, 1.0f }	};
		outMat.Load(trans);
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
		outMat.Load(rot);
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
		outMat.Load(rot);
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
		outMat.Load(rot);
	}

	inline void BuildCombinedRot4x4Matrix(const float* inRot, Matrix4X4 &outMat)
	{

	}

}