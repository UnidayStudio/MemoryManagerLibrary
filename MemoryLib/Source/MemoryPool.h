#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <vector>
#include <memory>
#include <iostream>

#define DEFAULT_POOL_SIZE 1000

#define DEBUG(X) //std::cout << "[Debug] " << X << std::endl;

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
			
			for (unsigned int i = 0; i < m_arrays.size(); i++) {
				DEBUG("Deallocating "<< i)
				for (int j = 0; j < m_poolSize; j++) {
					// This may cause some problems later on...
					// (calling the dtor of a destroyed obj)
					m_allocator.destroy(&m_arrays[i][j]);
				}
				m_allocator.deallocate(&m_arrays[i][0], m_poolSize);

				m_freeSlots[i].clear();
			}
			m_arrays.clear();
			m_freeSlots.clear();
		}

		T* Add() {
			for (unsigned int pool = 0; pool < m_freeSlots.size(); pool++) {
				if (m_freeSlots[pool].size() > 0) {
					return AddToPool(pool);
				}
			}
			AddPool();
			return AddToPool(m_freeSlots.size()-1);
		}

		

		void Destroy(T* element) {
			for (unsigned int i = 0; i < m_arrays.size(); i++) {
				if (element < &m_arrays[i][0] || element > &m_arrays[i][m_poolSize - 1])
					continue;

				int id = element - &m_arrays[i][0];

				m_freeSlots[i].push_back(id);
				m_allocator.destroy(&m_arrays[i][id]);

				DEBUG("Removed an element: " << i << ", " << id)
				return;
			}
		}

	private:
		std::vector<T*> m_arrays;
		std::vector< std::vector<int> > m_freeSlots;
		std::allocator<T> m_allocator;	

		int m_poolSize;

		void AddPool() {
			//DEBUG("Max size: " <<m_allocator.max_size())
			m_arrays.emplace_back(m_allocator.allocate(m_poolSize));


			m_freeSlots.resize(m_freeSlots.size() + 1);
			m_freeSlots.back().resize(m_poolSize);

			bool* tmp = new bool[m_poolSize];
			for (int i = 0; i < m_poolSize; i++) {
				tmp[i] = false;
				m_freeSlots.back()[i] = m_poolSize - i - 1;
			}
			DEBUG("New pool Added!")
		}

		T* AddToPool(int pool) {
			int slot = m_freeSlots[pool].back();

			m_allocator.construct(&m_arrays[pool][slot]);
			m_freeSlots[pool].pop_back();

			DEBUG("Added on an existing pool:" << pool << ", " << slot)
			return &m_arrays[pool][slot];
		}
	};

}

#endif // MEMORY_POOL_H