#include "MemoryAccess.h"

#include "MemoryManager.h"
#include "MemoryPool.h"

namespace memTest {
	void MemoryAccess() {
		mem::Pool<int> data(10);

		int* x[20];
		for (int i = 0; i < 15; i++) {
			x[i] = data.Add();
		}

		data.Destroy(x[2]);
		data.Destroy(x[7]);
		data.Destroy(x[11]);

		int* b = data.Add();
	}
}