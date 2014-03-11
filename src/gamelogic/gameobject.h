#ifndef _GameLogic_GameObject_H_
#define _GameLogic_GameObject_H_

#include "component.h"

#include "common/t4type.h"
#include "common/eventdispatcher.h"
#include "common/atomstring.h"

#include <vector>

namespace t4
{
	namespace gamelogic
	{
		struct GameObject{};
		typedef CAtomString<GameObject> CGameObjectAtom;

		class CGameObjectFactory;
		class CGameObject
		{
			friend class CGameObjectFactory;
		public:
			CGameObjectAtom Type() const
			{
				return m_atomTypeName;
			}
			UINT32 Identity() const
			{
				return m_dwIdentity;
			}

			template<typename ComponentType>
			ComponentType *GetComponent(CComponentAtom name = ComponentType::asName)
			{
				auto it = m_mapComponents.find(name);
				if (it == m_mapComponents.end())
					return 0;

				return it->second;
			}

		private:
			void Attach(IComponent &oComponent)
			{
				m_vecComponents.push_back(&oComponent);
				m_mapComponents[oComponent.Name()] = &oComponent;
				oComponent.OnAttach(m_oEventMgr);
			}

			std::vector<IComponent *> m_vecComponents;
			stdext::hash_map<CComponentAtom, IComponent*> m_mapComponents;
			CEventManager m_oEventMgr;
			CGameObjectAtom m_atomTypeName;
			UINT32 m_dwIdentity;
		};

		class CGameObjectFactory
		{
			IComponentFactory &m_oComponentFactory;
			stdext::hash_map<CGameObjectAtom, std::vector<CComponentAtom> > m_mapDefines;
			UINT32 m_dwLastIdentity;
		public:
			CGameObjectFactory(IComponentFactory &oComponentFactory) : m_oComponentFactory(oComponentFactory), m_dwLastIdentity(0)
			{
			}
			CGameObject *Create(CGameObjectAtom name);
			void Release(CGameObject &oObj);
		};
	}
}
#endif//_GameLogic_GameObject_H_
