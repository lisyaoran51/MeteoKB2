#include "KeyBindingStore.h"


using namespace Framework::Input::KeyBindings;



KeyBindingStore::KeyBindingStore(function<DatabaseContext*(void)> gContext, Storage * s): DatabaseStore(gContext, s), RegisterType("KeyBindingStore")
{
	// 應該部會用到
}
