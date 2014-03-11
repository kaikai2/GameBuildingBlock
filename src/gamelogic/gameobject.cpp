#include "gameobject.h"

#include "common/pool.h"

namespace t4
{
	namespace gamelogic
	{
		CGameObject *CGameObjectFactory::Create(CGameObjectAtom name)
		{
			if (CGameObject *poObj = pool::Alloc<CGameObject>())
			{
				const auto it = m_mapDefines.find(name);
				if (it != m_mapDefines.end())
				{
					const auto &v = it->second;
					std::vector<IComponent *> vecComponents;
					vecComponents.reserve(v.size());
					for (size_t i = 0; i < v.size(); ++i)
					{
						auto c = m_oComponentFactory.Create(v[i]);
						if (!c)
							break;
						vecComponents.push_back(c);
					}
					if (vecComponents.size() != v.size())
					{
						for (size_t i = 0; i < vecComponents.size(); ++i)
						{
							m_oComponentFactory.Release(vecComponents[i]);
						}
						pool::Free(poObj);
						return 0;
					}
					for (size_t i = 0; i < vecComponents.size(); ++i)
					{
						poObj->Attach(*vecComponents[i]);
					}
				}
				return poObj;
			}
			return 0;

		}
		void CGameObjectFactory::Release(CGameObject &oObj)
		{
			pool::Free(&oObj);
		}
	}
}