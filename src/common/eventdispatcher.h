#ifndef _Common_EventDispatcher_H_
#define _Common_EventDispatcher_H_

#include "atomstring.h"
#include "cast.h"

#include <hash_map>

namespace t4
{
	struct Event{};
	typedef CAtomString<Event> CEventAtom;

	class IEventHandler
	{
	public:
		virtual ~IEventHandler() = 0;

		typedef void (IEventHandler::* Handler)(CEventAtom key, const void *attach, int attachLen);
	};

	inline IEventHandler::~IEventHandler()
	{

	}

	class IEventDispatcher
	{
	public:
		virtual ~IEventDispatcher() = 0;

		template<typename EventHandler, typename Handler>
		void Bind(CEventAtom key, EventHandler *poHandler, Handler pFunc)
		{
			Bind(key, strong_cast<IEventHandler*>(poHandler),
				strong_cast<IEventHandler::Handler>(pFunc));
		}
		virtual void Bind(CEventAtom key, IEventHandler *poHandler, IEventHandler::Handler pFunc) = 0;

		virtual void Send(CEventAtom key, const void *pAttach, int attachLen) = 0;
	};

	inline IEventDispatcher::~IEventDispatcher()
	{
	}

	class CEventManager : public IEventDispatcher
	{
	public:

		virtual void Bind(CEventAtom key, IEventHandler *poHandler, IEventHandler::Handler pFunc);

		virtual void Send(CEventAtom key, const void *pAttach, int attachLen);
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
