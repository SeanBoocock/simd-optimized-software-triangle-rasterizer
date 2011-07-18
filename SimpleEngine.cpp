
#include "SimpleEngine.h"
#include "Window.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Primitive.h"
#include "Vector.h"
#include <stdio.h>
#include "Debugging.h"
#include "Timer.h"
#include "tbb/task_scheduler_init.h"


SimpleEngine::SimpleEngine()	:	win(nullptr),
									m("pot4.asc")
{
	DEBUG_PRINT("SimpleEngine constructed.\n");
}
SimpleEngine::~SimpleEngine()
{
}

void SimpleEngine::Initialize()
{
	tbb::task_scheduler_init init;
	win = new Window();
	win->Initialize();

	m.LoadMesh();
	//m.LoadTestCube();

	RENDERER->SubmitVertexBuffer(&m.GetBuffer());
}

void SimpleEngine::Run()
{
	printf("Engine Run\n");
	Timer t;
	double frameTime = 0.0;
	while(win->MessagePump())
	{
		t.reset();
		if( win->Present( RENDERER->Draw() ) != S_OK )
			break;
		frameTime = t.getMilliseconds();
		printf("Frame time for last frame was: %fms.\n", frameTime);
	}

}

void SimpleEngine::Finalize()
{
	printf("Engine Run\n");
	delete win;
}