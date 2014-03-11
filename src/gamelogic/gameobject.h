#ifndef _GameLogic_GameObject_H_
#define _GameLogic_GameObject_H_

#include "component.h"

#include "common/t4type.h"

#include "common/eventdispatcher.h"

#include <vector>

namespace t4
{
	namespace gamelogic
	{
		class CGameObject
		{
		public:

			UINT32 Identity() const
			{
				return m_dwIdentity;
			}

			void Attach(IComponent &oComponent)
			{
				m_vecComponents.push_back(&oComponent);
				oComponent.OnAttach(m_oEventMgr);
			}

		private:

			std::vector<IComponent *> m_vecComponents;

			CEventManager m_oEventMgr;

			UINT32 m_dwIdentity;
		};

		class CGameObjectFactory
		{

		};
	}
}
#endif//_GameLogic_GameObject_H_
