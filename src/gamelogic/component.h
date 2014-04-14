#ifndef _GameLogic_Component_H_
#define _GameLogic_Component_H_

#include "common/eventdispatcher.h"
#include "common/atomstring.h"
#include "common/pool.h"
#include "common/attributeset.h"

#include <cassert>

namespace t4
{
	namespace gamelogic
	{
		struct Component{};
		typedef CAtomString<Component> CComponentAtom;

		class CGameObject;

		class IComponent : protected IAttributeHolder
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
			friend class CGameObject;
			friend class CAttributeSet;

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

		class CComponentFactory : public IComponentFactory
		{
			template<typename ComponentType>
			class Creator : public IComponentFactory
			{
			public:
				virtual IComponent *Create(CComponentAtom name)
				{
					return pool::Alloc<ComponentType>();
				}

				virtual void Release(IComponent *poNode)
				{
					if (ComponentType *poComponent = dynamic_cast<ComponentType *>(poNode))
					{
						pool::Free(poComponent);
					}
				}
			};
		public:
			template<typename ComponentType>
			void Register(CComponentAtom name)
			{
				IComponentFactory *pFactory = new Creator<ComponentType>();
				if (!m_mapFactory.insert(std::make_pair(name, pFactory)).second)
				{
					delete pFactory;
				}
			}
			virtual IComponent *Create(CComponentAtom name);

			virtual void Release(IComponent *poNode);
		private:
			stdext::hash_map<CComponentAtom, IComponentFactory*> m_mapFactory;
		};
	}
}

#endif//_GameLogic_Component_H_
