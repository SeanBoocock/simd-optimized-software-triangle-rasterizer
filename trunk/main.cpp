
#include "SimpleEngine.h"

int main()
{
	SimpleEngine engine;

	engine.Initialize();
	engine.Run();
	engine.Finalize();

	return 0;
}