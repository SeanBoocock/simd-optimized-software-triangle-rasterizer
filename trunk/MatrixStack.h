
#ifndef MATRIX_STACK_H_
#define MATRIX_STACK_H_

#include "MatrixStackBase.h"
#include "Alignment.h"

using namespace Math;

template<typename Enum, unsigned short Size = 3>
class MatrixStack : public MatrixStackBase
{
public:
	MatrixStack()	:	stackDescription((Enum)0)
	{
		
	}

	~MatrixStack()
	{

	}

	ALIGNED_NEW


	void Push(Matrix4X4 &incMat)
	{
		if(stackPointer)
		{
			Matrix4X4Multiply( stack[stackPointer-1], incMat, stack[stackPointer]);
		}
		else
		{
			if(below)
				Matrix4X4Multiply( below->Top(), incMat, stack[stackPointer] );
			else
			{
				stack[0][0] = incMat[0];
				stack[0][1] = incMat[1];
				stack[0][2] = incMat[2];
				stack[0][3] = incMat[3];
			}
		}
		++stackPointer;
		stackDescription = (Enum)(stackDescription + 1);
	}

	void Pop()
	{
		assert(stackPointer>0);
		--stackPointer;
		stackDescription = (Enum)(stackDescription - 1);
	}


	const Matrix4X4& Top () const
	{
		return stack[stackPointer-1];
	}

	Enum TopType() const
	{
		return stackDescription;
	}


private:
	Matrix4X4 stack[Size];
	Enum stackDescription;
};

#endif