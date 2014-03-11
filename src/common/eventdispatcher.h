#ifndef _Common_EventDispatcher_H_
#define _Common_EventDispatcher_H_

#include "atomstring.h"

#include <hash_map>

namespace t4
{
	struct Event{};
	typedef CAtomString<Event> CEventAtom;
}

namespace stdext
{
	template<> inline
		size_t hash_value(const t4::CEventAtom& _Keyval)
	{	// hash _Keyval to size_t value one-to-one
			return ((size_t)_Keyval ^ _HASH_SEED);
		}
}

namespace t4
{
	class IEventHandler
	{
	public:
		virtual ~IEventHandler() = 0;

		typedef void (IEventHandler::* Handler)(CEventAtom key, const void *attach, int attachLen);

		void init()
		{
			Handler t = &IEventHandler::foo;
		}
		void foo(CEventAtom key, const void *attach, int attachLen);

	};

	inline IEventHandler::~IEventHandler()
	{

	}

	class IEventDispatcher
	{
	public:
		virtual ~IEventDispatcher() = 0;

		virtual void Bind(CEventAtom key, IEventHandler *poHandler, IEventHandler::Handler pFunc) = 0;

		virtual void Send(CEventAtom key, const void *pAttach, int attachLen) = 0;
	};

	inline IEventDispatcher::~IEventDispatcher()
	{
	}

	class CEventManager : public IEventDispatcher
	{
	public:
		virtual void Bind(CEventAtom key, IEventHandler *poHandler, IEventHandler::Handler pFunc)
		{
			m_mapHandlers.insert(std::make_pair(key, Callback() = {poHandler, pFunc }));
		}

		virtual void Send(CEventAtom key, const void *pAttach, int attachLen)
		{
			auto cc = m_mapHandlers.equal_range(key);
			if (cc.first != cc.second)
			{
				for (auto i = cc.first; i != cc.second; ++i)
				{
					auto &cb = i->second;
					(cb.obj->*(cb.func))(key, pAttach, attachLen);
				}
			}
		}
	private:
		struct Callback
		{
			IEventHandler* obj;
			IEventHandler::Handler func;
		};
		typedef stdext::hash_multimap<CEventAtom, Callback> MapHandler;
		MapHandler m_mapHandlers;
	};
}


#endif//_Common_EventDispatcher_H_
