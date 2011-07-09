
#ifndef ALIGNMENT_H_
#define ALIGNMENT_H_

#include <malloc.h>
#include <new>

#define DEFAULT_ALIGNMENT 16

#define ALIGN __declspec(align(DEFAULT_ALIGNMENT))
#define ALIGN_TO(x) __declspec(align(x))
#define ALIGNED_NEW  void* operator new(size_t sz) throw (...)		\
				{														\
					void* m = _aligned_malloc(sz, DEFAULT_ALIGNMENT);	\
					if (!m) throw std::bad_alloc();						\
					return m;											\
				}														\
																		\
				void operator delete(void* m)							\
				{														\
					_aligned_free(m);									\
				}														


#endif