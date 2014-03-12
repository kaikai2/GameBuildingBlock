#ifndef _Common_Pool_H_
#define _Common_Pool_H_

#include <cassert>

namespace t4
{
	namespace pool
	{
		template<typename ElementType>
		class CDynamicPool
		{
		public:
			CDynamicPool() : m_nTotalCount(0), m_nInitCount(128), m_nGrowCount(128)
			{
			}
			~CDynamicPool()
			{
				assert(m_nTotalCount == m_listFree.size());
				for (size_t i = 0; i < m_vecPools.size(); ++i)
				{
					free(m_vecPools[i]);
				}
				m_vecPools.clear();
				m_listFree.clear();
			}
			void Set(size_t initCount, size_t growCount)
			{
				if (m_nTotalCount == 0)
				{
					m_nInitCount = initCount;
					m_nGrowCount = growCount;
				}
			}
			ElementType *Alloc()
			{
				if (m_listFree.empty())
				{
					Grow();
				}
				if (m_listFree.empty())
				{
					return 0;
				}
				ElementType *poNode = m_listFree.front();
				m_listFree.pop_front();
				new (poNode)ElementType();
				return poNode;
			}
			void Free(ElementType *poNode)
			{
				poNode->~ElementType();
				m_listFree.push_back(poNode);
			}
		private:
			void Grow()
			{
				int count = m_vecPools.empty() ? m_nInitCount : m_nGrowCount;
				ElementType *poBuff = (ElementType *)malloc(sizeof(ElementType) * count);
				for (int i = 0; i < count; i++)
				{
					m_listFree.push_back(poBuff + i);
				}
				m_nTotalCount += count;
				m_vecPools.push_back(poBuff);
			}
			size_t m_nTotalCount;
			size_t m_nInitCount;
			size_t m_nGrowCount;
			std::list<ElementType*> m_listFree;
			std::vector<ElementType*> m_vecPools;
		};

		template<typename ElementType>
		CDynamicPool<ElementType> &Pool()
		{
			static CDynamicPool<ElementType> s_pool;
			return s_pool;
		}

		template<typename ElementType>
		void Set(size_t initCount, size_t growCount)
		{
			Pool<ElementType>().Set(initCount, growCount);
		}

		template<typename ElementType>
		ElementType* Alloc()
		{
			return Pool<ElementType>().Alloc();
		}
		template<typename ElementType>
		void Free(ElementType *poNode)
		{
			Pool<ElementType>().Free(poNode);
		}
	};
}


#endif//_Common_Pool_H_
