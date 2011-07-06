
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
		stack[stackPointer++] = (stackPointer) ? Matrix4X4Multiply( stack[stackPointer-1], incMat ) : ( below ? Matrix4X4Multiply( below->Top(), incMat ) : incMat );
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