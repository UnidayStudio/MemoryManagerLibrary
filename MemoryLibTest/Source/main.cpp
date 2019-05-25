#include <iostream>

#include "MemoryManager.h"
#include "MemoryPool.h"

#include "Tests\ExecutionTime.h"
#include "Tests\MemoryAccess.h"

int main() {
	mem::PrintInfo();

	//memTest::MemoryAccess();
	memTest::ExecutionTime(10);

	std::cout << "(Press something to exit)\n";
	
	std::cin.get();
	return 0;
}