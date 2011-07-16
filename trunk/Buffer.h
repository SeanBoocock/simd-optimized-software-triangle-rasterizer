
#ifndef BUFFER_H_
#define BUFFER_H_

//#include "PrimitiveBase.h"

class Buffer
{
public:
	Buffer(){}
	virtual ~Buffer(){}

	/*virtual PrimitiveBase* At(unsigned int index) = 0;*/

	virtual const unsigned int Size() const = 0;

protected:

};

#endif