#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#define DEFAULT_POOL_SIZE 1000

#define DEBUG(X) //std::cout << "[Debug] " << X << std::endl;

#include <vector>
#include <memory>

namespace mem {

	template <class T>
	class Pool {
	public:
		Pool() : m_poolSize(DEFAULT_POOL_SIZE){
			AddPool();
		}
		Pool(int poolSize) : m_poolSize(poolSize) {
			AddPool();
		}
		virtual ~Pool() {
			// Final Garbage Collector
			
			for (int i = 0; i < m_arrays.size(); i++) {
				DEBUG("Deallocating "<< i)
				for (int j = 0; j < m_poolSize; j++) {
					if(m_lockedSlots[i][j])
						m_allocator.destroy(&m_arrays[i][j]);
				}

				delete[] m_lockedSlots[i];
				m_allocator.deallocate(&m_arrays[i][0], m_poolSize);
			}
			
			m_arrays.clear();
			m_lockedSlots.clear();
			m_usedSlots.clear();
			m_nextFreeSlot.clear();

		}

		T* Add() {
			int len;

			for (int pool = 0; pool < m_usedSlots.size(); pool++) {
				if (m_usedSlots[pool] < m_poolSize) {
					len = m_poolSize;

					for (int j = m_nextFreeSlot[pool]; j < len; j++) {
						if (m_lockedSlots[pool][j] == false) {
							m_lockedSlots[pool][j] = true;
							m_usedSlots[pool]++;
							m_nextFreeSlot[pool] = j + 1;

							m_allocator.construct(&m_arrays[pool][j]);

							DEBUG("Added on an existing pool:" << pool << ", " << j)
							return &m_arrays[pool][j];
						}
						if (j == m_poolSize - 1) {
							j = 0;
							len = m_nextFreeSlot[pool];
							DEBUG("Add: Restarting the list search...")
						}
					}
				}
			}
			AddPool();
			m_lockedSlots.back()[0] = true;
			m_usedSlots.back()++;
			m_nextFreeSlot.back() = 1;
			m_allocator.construct(&m_arrays.back()[0]);

			DEBUG("Added on an new pool!")
			return &m_arrays.back()[0];
		}

		void Destroy(T* element) {
			for (int i = 0; i < m_arrays.size(); i++) {
				if (element < &m_arrays[i][0] || element > &m_arrays[i][m_poolSize - 1])
					continue;

				int id = element - &m_arrays[i][0];
				m_lockedSlots[i][id] = false;
				m_usedSlots[i]--;
				m_nextFreeSlot[i] = id;

				m_allocator.destroy(&m_arrays[i][id]);

				DEBUG("Removed an element: " << i << ", " << id)
				return;
			}
		}

	private:
		std::vector<T*> m_arrays;
		std::vector<bool*> m_lockedSlots;
		std::vector<unsigned int> m_usedSlots;
		std::vector<int> m_nextFreeSlot;

		std::allocator<T> m_allocator;	

		int m_poolSize;

		void AddPool() {
			DEBUG("Max size: " <<m_allocator.max_size())
			m_arrays.emplace_back(m_allocator.allocate(m_poolSize));

			bool* tmp = new bool[m_poolSize];
			for (int i = 0; i < m_poolSize; i++)
				tmp[i] = false;

			m_lockedSlots.emplace_back(tmp);
			m_usedSlots.emplace_back(0);
			m_nextFreeSlot.emplace_back(0);

			DEBUG("New pool Added!")
		}
	};

}

#endif // MEMORY_POOL_H