#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <typeinfo>



namespace mem {
	void PrintInfo();

	// TODO...
	/*
	template <typename T>
	T* spawn() {
		T* x = new T;
		return x;
	}

	template<typename T>
	void destroy(T& data) {
		delete data;
	}*/
}

#endif // MEMORY_MANAGER_H