
#include "Vector.h"



namespace Math
{

	inline Vector4 Vec3DotVec3(const Vector4 &vec1, const Vector4 &vec2)
	{
		Vector4 toReturn;
	#if 0
		Vector4 temp1, temp2;
		temp1 = _mm_mul_ps(vec1,vec2); //vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(0, 0, 0, 0));
		toReturn = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(1, 1, 1, 1));
		toReturn =_mm_add_ps(temp2, toReturn);
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(2, 2, 2, 2));
		toReturn =_mm_add_ps(temp2, toReturn);
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 3, 3, 3));
		return toReturn = _mm_add_ps(toReturn,temp2);
	#endif
		return toReturn = _mm_dp_ps(vec1,vec2,127);
	}
	
	inline Vector4 Vec4DotVec4(const Vector4 &vec1, const Vector4 &vec2)
	{
		Vector4 toReturn;
	#if 0
		Vector4 temp1, temp2;
		temp1 = _mm_mul_ps(vec1,vec2); //vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(0, 0, 0, 0));
		toReturn = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(1, 1, 1, 1));
		toReturn =_mm_add_ps(temp2, toReturn);
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(2, 2, 2, 2));
		toReturn =_mm_add_ps(temp2, toReturn);
		temp2 = _mm_shuffle_ps(temp1, temp1, _MM_SHUFFLE(3, 3, 3, 3));
		return toReturn = _mm_add_ps(toReturn,temp2);
	#endif
		return toReturn = _mm_dp_ps(vec1,vec2,255);
	}

	inline Vector4 Vec3CrossVec3(const Vector4 &vec1, const Vector4 &vec2)
	{
		Vector4 toReturn, temp1;
		temp1 = _mm_shuffle_ps(vec1, vec2, _MM_SHUFFLE(2, 0, 2, 1)); 
		toReturn = _mm_shuffle_ps(vec2, vec1, _MM_SHUFFLE(0, 2, 1, 2));
		temp1 = _mm_mul_ps(temp1,toReturn);
		toReturn = _mm_shuffle_ps(vec2, vec1, _MM_SHUFFLE(2, 3, 0, 1));
		toReturn = _mm_mul_ps(vec1, toReturn);
		return toReturn =  _mm_hsub_ps(temp1,toReturn);
	}

	inline void Normalize(Vector4 &vec) //Not dealing with last component
	{
		if( ( _mm_movemask_ps( _mm_cmpeq_ps( vec, Math::zero )  ) & (VERT_X | VERT_Y | VERT_Z)  ) ==  (VERT_X | VERT_Y | VERT_Z)  )
			return;
		Vector4 temp,temp2,temp3;
		temp = Math::Vec3DotVec3(vec,vec);
		temp = _mm_insert_ps(temp,Math::ident,_MM_MK_INSERTPS_NDX(3,3,0));
		temp = _mm_rsqrt_ps(temp); 
		vec = _mm_mul_ps(vec,temp);
	}

	inline void ScaleForW(Vector4 &vec)
	{
		Vector4 temp = _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(3, 3, 3, 3));
		vec = _mm_div_ps(vec,temp);
	}

	inline Vector4 LoadVector4Unaligned(const float* inArray)
	{
		return Vector4(_mm_loadu_ps(inArray));
	}
	
	inline Vector4 LoadVector4Aligned(const float* inArray)
	{
		return _mm_load_ps(inArray);
	}

	Vector4 operator+ (Vector4 in)	{	return in;	}
	Vector4 operator+ (Vector4 in, Vector4 inTwo)	{	return _mm_add_ps(in,inTwo); }
	Vector4 operator- (Vector4 in)	{	return _mm_mul_ps(in, Math::negate); }
	Vector4 operator- (Vector4 in, Vector4 inTwo)	{ return _mm_sub_ps(in, inTwo);	}
	Vector4 operator+= (Vector4 inOne, Vector4 inTwo) { return _mm_add_ps(inOne,inTwo); }
	Vector4 operator-= (Vector4 subtractFrom, Vector4 subtractAmt) { return _mm_sub_ps(subtractFrom, subtractAmt); }

}//Namespace Math