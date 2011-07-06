
#ifndef SIMPLE_ENGINE_H_
#define SIMPLE_ENGINE_H_

class Window;

class SimpleEngine
{
public:
	SimpleEngine();
	~SimpleEngine();

	void Initialize();
	void Run();
	void Finalize();

private:
	Window* win;

};

#endif