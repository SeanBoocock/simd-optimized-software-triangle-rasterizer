
#ifndef PIPELINE_BASE_H_
#define PIPELINE_BASE_H_

#include "PrimitiveBase.h"

enum PipelineStatus	: unsigned short
{
	PIPELINE_UNINITIALIZED = 0,
	PIPELINE_STARTED = 1,
	PIPELINE_RUNNING = 2,
	PIPELINE_ABORTED = 4,
	PIPELINE_PAUSED = 8,
	PIPELINE_FINISHED = 16
};

//This is all intended for later.  Primitives will carry a pointer to PipelineStageBase that is an instance of PipelineStage.
//The idea is that I will be able to have primitives work their way through the pipeline by themselves.  Tied in with the threadpool
//work scheduling, a primitive will initially be scheduled, execute its current
class PipelineBase
{
public:
	PipelineBase(){}
	virtual ~PipelineBase(){}
	
	virtual void Execute(PrimitiveBase*) = 0;
	virtual void Iterate() = 0;
	virtual unsigned short GetStatus() = 0;
	virtual void Pause() = 0;
	virtual void Restart() = 0;
};

#endif
