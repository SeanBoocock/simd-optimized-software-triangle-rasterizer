/*
 *  Singleton.h
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename T>
class Singleton
{
public:
	static void CreateInstance()
	{
			if (singletonInstance == 0) {
				singletonInstance = new T();
			}
	}
	static T* GetInstancePtr()
	{
		if (singletonInstance == 0) {
			singletonInstance = new T();
		}
		return singletonInstance;
	}
	static void Destroy()
	{
		delete singletonInstance;
		singletonInstance = 0;
	}
	
protected:
	Singleton () {}
	virtual ~Singleton() {}
private:
	static T* singletonInstance;
	Singleton(const Singleton& single) {}         // Prevent copy-construction
	Singleton& operator=(const Singleton&);      // Prevent assignment
};

template <typename T> T* Singleton<T>::singletonInstance = NULL;


#endif