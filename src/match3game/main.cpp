#define NOMINMAX
#include <windows.h>

#include "gamelogic/gameobject.h"
#include "gamelogic/scene.h"

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
		assert(attach == 0 && attachLen == 2);
	}
};
CComponentAtom SomeWhatComponent::asName("SomeWhat");

CGameObjectAtom atomBrick("brick");

#undef min
#undef max

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

	gameObjectFactory.Define(atomBrick).With(SomeWhatComponent::asName);

	CScene oScene;
	if (CGameObject *poBrick = gameObjectFactory.Create(atomBrick))
	{
		oScene.Enter(*poBrick);

		assert(oScene.Has(*poBrick));

		oScene.Tag().AddTag(*poBrick, CSceneTagAtom("id"), 1);
		oScene.Tag().SetTag(*poBrick, CSceneTagAtom("type"), 2);

		assert(poBrick == oScene.Tag().$().HasKey(CSceneTagAtom("type")).Value(CSceneTagAtom("id"), 1));
		poBrick->Send(CEventAtom("someEvent"), 0, 2);

		oScene.Leave(*poBrick);
		gameObjectFactory.Release(*poBrick);
	}

	return 0;
}