#include "component.h"

namespace t4
{
	namespace gamelogic
	{

		IComponent *CComponentFactory::Create(CComponentAtom name)
		{
			auto it = m_mapFactory.find(name);

			if (it != m_mapFactory.end())
			{
				return it->second->Create(name);
			}
			return 0;
		}

		void CComponentFactory::Release(IComponent *poNode)
		{
			auto name = poNode->Name();
			auto it = m_mapFactory.find(name);

			if (it == m_mapFactory.end())
			{
				assert(0);
				return;
			}
			it->second->Release(poNode);
		}
	}
}