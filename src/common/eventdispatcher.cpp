#include "eventdispatcher.h"

namespace t4
{

	void CEventManager::Bind(CEventAtom key, IEventHandler *poHandler, IEventHandler::Handler pFunc)
	{
		Callback cb = {poHandler, pFunc};
		m_mapHandlers.insert(std::make_pair(key, cb));
	}

	void CEventManager::Send(CEventAtom key, const void *pAttach, int attachLen)
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
}