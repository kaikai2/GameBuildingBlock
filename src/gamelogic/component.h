#ifndef _GameLogic_Component_H_
#define _GameLogic_Component_H_

#include "common/eventdispatcher.h"
#include "common/atomstring.h"

namespace t4
{
	namespace gamelogic
	{
		struct Component{};
		typedef CAtomString<Component> CComponentAtom;

		class CGameObject;

		class IComponent
		{
		public:
			virtual ~IComponent() = 0;
			
			virtual CComponentAtom Name() const = 0;

			virtual void OnAttach(IEventDispatcher &oEventDispatcher) = 0;
		protected:
			CGameObject &GameObject()
			{
				return *m_poGameObject;
			}
			const CGameObject &GameObject() const
			{
				return *m_poGameObject;
			}

		private:
			CGameObject *m_poGameObject;
		};

		inline IComponent::~IComponent()
		{
		}


		class IComponentFactory
		{
		public:
			virtual ~IComponentFactory() = 0;

			virtual IComponent *Create(CComponentAtom name) = 0;

			virtual void Release(IComponent *) = 0;
		};

		inline IComponentFactory::~IComponentFactory()
		{
		}
	}
}

#endif//_GameLogic_Component_H_
