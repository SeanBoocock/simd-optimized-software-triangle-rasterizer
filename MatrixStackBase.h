
#ifndef MATRIX_STACK_BASE_H_
#define MATRIX_STACK_BASE_H_

#include "MatrixMath.h"
#include <assert.h>

using namespace Math;

class MatrixStackBase
{
public:
	MatrixStackBase()	:	stackPointer(0),
							above(nullptr),
							below(nullptr)
	{
	}

	virtual ~MatrixStackBase(){}

	virtual void Push(Matrix4X4 &incMat) = 0;

	void Pop()
	{
		assert(stackPointer>0);
		--stackPointer;
	}

	void Reset()
	{
		stackPointer = 0;
	}

	virtual const Matrix4X4& Top () const = 0;

	MatrixStackBase* Above()	{	return above; }
	void SetStackAbove(MatrixStackBase* incAbove)
	{
		above = incAbove;
	}

	MatrixStackBase* Below()	{	return below; }
	void SetStackBelow(MatrixStackBase* incBelow)
	{
		below = incBelow;
	}

protected:
	unsigned short stackPointer;
	MatrixStackBase* below;
	MatrixStackBase* above;
};

#endif