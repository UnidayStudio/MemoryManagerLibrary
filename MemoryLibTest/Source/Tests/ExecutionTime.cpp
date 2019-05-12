#include "ExecutionTime.h"

#include <iostream>
#include <memory>
#include <chrono>

#include "MemoryManager.h"
#include "MemoryPool.h"

#define NEW_SYSTEM

#define TEST_SIZE 10000

#ifdef NEW_SYSTEM // My system...
#define NEW(d, t) d.Add()
#define DELETE(d, p) d.Destroy(p)

#else // Current new and delete
#define NEW(d, t) new t
#define DELETE(d, p) delete p
#endif

namespace memTest {
	void ExecutionTime(int iterations) {
		float avgTime1 = 0;
		float avgTime2 = 0;
		float avgTime3 = 0;

		for (int tests = 0; tests < iterations; tests++) {
			std::cout << "Running test... " << tests << " of " << iterations << "...\n";



#ifdef NEW_SYSTEM
			mem::Pool<int> data;
#else
#define data void
#endif

			int* x[TEST_SIZE];// = data.Add();

							  // Record start time
			auto start = std::chrono::high_resolution_clock::now();

			// Creating...
			//std::cout << "\tPart 1 (allocating)...\n";
			for (int i = 0; i < TEST_SIZE; i++) {
				x[i] = NEW(data, int); //data.Add();
			}

			// Record end time
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;

			//std::cout << "\tElapsed time: " << elapsed.count() << " s\n";
			avgTime1 += elapsed.count();


			start = std::chrono::high_resolution_clock::now();
			// Doing some maths...
			//std::cout << "\tPart 2 (maths)...\n";
			for (int j = 0; j < 100; j++) {
				for (int i = 0; i < TEST_SIZE; i++) {
					*x[i] = i;
				}
			}
			finish = std::chrono::high_resolution_clock::now();
			elapsed = finish - start;
			avgTime2 += elapsed.count();


			start = std::chrono::high_resolution_clock::now();
			// Deleting...
			//std::cout << "\tPart 3 (deleting)...\n";
			for (int i = 0; i < TEST_SIZE; i++) {
				//data.Destroy(x[i]);
				DELETE(data, x[i]);
			}

			finish = std::chrono::high_resolution_clock::now();
			elapsed = finish - start;
			avgTime3 += elapsed.count();

		}

		std::cout << "\nFinished! \nAverage times: \n";
		std::cout << "\t Allocation: " << avgTime1 / iterations << " s \n";
		std::cout << "\t Iteration: " << avgTime2 / iterations << " s \n";
		std::cout << "\t Deallocation: " << avgTime3 / iterations << " s \n";
	}
}