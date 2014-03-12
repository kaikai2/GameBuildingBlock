
#include <windows.h>

#include "gamelogic/gameobject.h"

using namespace t4;
using namespace t4::gamelogic;

class SomeWhatComponent : public IComponent
{
public:
	static CComponentAtom asName;
	virtual CComponentAtom Name() const
	{
		return asName;
	}

	virtual void OnAttach(IEventDispatcher &oEventDispatcher)
	{
		oEventDispatcher.Bind(CEventAtom("someEvent"), this, &SomeWhatComponent::foo);
	}

	void foo(CEventAtom key, const void *attach, int attachLen)
	{

	}
};
CComponentAtom SomeWhatComponent::asName("SomeWhat");

CGameObjectAtom atomBrick("brick");
int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	CComponentFactory componentFactory;
	CGameObjectFactory gameObjectFactory(componentFactory);

	componentFactory.Register<SomeWhatComponent>(SomeWhatComponent::asName);

	if (CGameObject *poBrick = gameObjectFactory.Create(atomBrick))
	{
		
		gameObjectFactory.Release(*poBrick);
	}

	return 0;
}