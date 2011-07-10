#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <xmmintrin.h>
#include <smmintrin.h>
#include "Alignment.h"
#include <float.h>

namespace Math
{

	typedef __m128 Vector4;

	extern Vector4 Vec3DotVec3(const Vector4 &vec1, const Vector4 &vec2);
	
	extern Vector4 Vec4DotVec4(const Vector4 &vec1, const Vector4 &vec2);

	extern Vector4 Vec3CrossVec3(const Vector4 &vec1, const Vector4 &vec2);

	extern void Normalize(Vector4 &vec);

	extern void ScaleForW(Vector4 &vec);

	extern Vector4 LoadVector4Unaligned(const float* inArray);
	
	extern Vector4 LoadVector4Aligned(const float* inArray);

	ALIGN struct Vector4Constant 
	{
		union 
		{
			float f[4];
			Vector4 v;
		};

		inline operator Vector4() const { return v; }
		inline operator __m128i() const { return reinterpret_cast<const __m128i *>(&v)[0]; }
		inline operator __m128d() const { return reinterpret_cast<const __m128d *>(&v)[0]; }

	};


	//Global constants
	static const Vector4Constant negate = { -1.0f, -1.0f, -1.0f, -1.0f };
	static const Vector4Constant flip1 = { -1.0f, 1.0f, 1.0f, 1.0f };
	static const Vector4Constant twice = { 2.0f, 2.0f, 2.0f, 2.0f };
	static const Vector4Constant zero = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const Vector4Constant zero1 = { 0.0f, 1.0f, 0.0f, 1.0f };
	static const Vector4Constant ident = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const Vector4Constant ident0 = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const Vector4Constant ident1 = { 0.0f, 0.0f, 1.0f, 0.0f };
	static const Vector4Constant ident2 = { 0.0f, 1.0f, 0.0f, 0.0f };
	static const Vector4Constant ident3 = { 1.0f, 0.0f, 0.0f, 0.0f };
	static const Vector4Constant half = { 0.5f, 0.5f, 0.0f, 0.0f };
	static const Vector4Constant colorScale = { 255.0f, 255.0f, 255.0f, 255.0f };
	static const Vector4Constant compareTrue = { FLT_MAX , FLT_MAX, FLT_MAX, FLT_MAX};

	//Operators
	Vector4 operator+ (Vector4 in);
	Vector4 operator+ (Vector4 in, Vector4 inTwo);
	Vector4 operator- (Vector4 in);
	Vector4 operator- (Vector4 in, Vector4 inTwo);
	Vector4 operator+= (Vector4 inOne, Vector4 inTwo);
	Vector4 operator-= (Vector4 subtractFrom, Vector4 subtractAmt);
	
	template<typename T>
	inline Vector4 operator*= (Vector4 toBeMultiplied, T amt)
	{
		Vector4 multAmt = _mm_load1_ps( & ( float(amt) ) );
		return _mm_mul_ps(toBeMultiplied, multAmt);
	}

	template<typename T>
	inline Vector4 operator* (T amt, Vector4 toBeMultiplied)
	{
		Vector4 multAmt = _mm_load1_ps( & ( float(amt) ) );
		return _mm_mul_ps(toBeMultiplied, multAmt);
	}
	template<>
	inline Vector4 operator* <Vector4>(Vector4 amt, Vector4 toBeMultiplied)
	{
		return _mm_mul_ps(toBeMultiplied,amt);
	}

}//namespace Math




#endif