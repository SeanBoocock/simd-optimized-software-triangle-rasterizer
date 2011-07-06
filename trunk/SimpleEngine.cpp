
#include "SimpleEngine.h"
#include "Window.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Primitive.h"
#include "Vector.h"
#include <stdio.h>
#include "Debugging.h"

SimpleEngine::SimpleEngine()	:	win(nullptr)
{
	DEBUG_PRINT("SimpleEngine constructed.\n");
}
SimpleEngine::~SimpleEngine()
{
}

void SimpleEngine::Initialize()
{
	win = new Window();
	win->Initialize();

	VertexBuffer* v = new VertexBuffer(1);
	PrimitiveBase* triangle = new Primitive<>();
	ALIGN float vert1[4] = { 0.0f, 0.01f, 0.01f, 1.0f };
	((Primitive<>*)triangle)->AddVertex( Math::LoadVector4Aligned( vert1 ) );
	ALIGN float vert2[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	((Primitive<>*)triangle)->AddVertex( Math::LoadVector4Aligned( vert2 ) );
	ALIGN float vert3[4] = { 0.01f, 1.0f, 0.02f, 1.0f };
	((Primitive<>*)triangle)->AddVertex( Math::LoadVector4Aligned( vert3 ) );
	v->FillBuffer(&triangle,1);

	RENDERER->SubmitVertexBuffer(v);
}

void SimpleEngine::Run()
{
	printf("Engine Run\n");
	while(win->MessagePump())
	{
		/*dostuff*/
		if( win->Present( RENDERER->Draw() ) != S_OK )
			break;
	}

}

void SimpleEngine::Finalize()
{
	printf("Engine Run\n");
	delete win;
}