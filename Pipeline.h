
#ifndef PIPELINE_STAGE_H_
#define PIPELINE_STAGE_H_

#include "Renderer.h"
#include "PipelineBase.h"
#include "PrimitiveBase.h"
#include "ErrorHandling.h"
#include "Debugging.h"

//http://www.parashift.com/c++-faq-lite/pointers-to-members.html#faq-33.13
#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

template<int StageCount = 4>
class Pipeline : public PipelineBase
{
public:
	Pipeline()	:	current(0),
					status(PIPELINE_UNINITIALIZED)
	{
		stages[0] = &Renderer::TransformVertices;
		stages[1] = &Renderer::ClipPrimitives;
		stages[2] = &Renderer::SortVertices;
		stages[3] = &Renderer::LEETest;
	}

	~Pipeline(){}

	void Execute(PrimitiveBase* primToExecute)
	{
		DEBUG_PRINT("Beginning execution of pipeline for primitive: %d.\n",primToExecute->ID());
		status = PIPELINE_STARTED | PIPELINE_RUNNING;
		current = 0;
		while( current != StageCount && !( (status & PIPELINE_PAUSED) == PIPELINE_PAUSED ) )
		{
			DEBUG_PRINT("Executing primitive: %d at stage: %d.\n",primToExecute->ID(), current);
			if( RENDERER_SUCCESS == CALL_MEMBER_FN(*RENDERER,stages[current])(primToExecute) )
			{
				DEBUG_PRINT("Primitive: %d at stage: %d finished successfully.\n",primToExecute->ID(), current);
				++current;
			}
			else
			{
				DEBUG_PRINT("Primitive: %d at stage: %d failed and is aborting.\n",primToExecute->ID(), current);
				status = PIPELINE_ABORTED;
				break;
			}
		}
		if( ( status & PIPELINE_RUNNING ) == PIPELINE_RUNNING )
			status = PIPELINE_FINISHED;
	}

	void Iterate()
	{
		++current;
	}

	unsigned short GetStatus()
	{
		return status;
	}

	void Pause()
	{ 
		status ^= PIPELINE_RUNNING | PIPELINE_PAUSED;
	}

	void Restart()
	{
		status ^= PIPELINE_RUNNING | PIPELINE_PAUSED;
	}

private:
	StagePtr stages[StageCount];
	unsigned short current;
	unsigned short status;
};

#endif