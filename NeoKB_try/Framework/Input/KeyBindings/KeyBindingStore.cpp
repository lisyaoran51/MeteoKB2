#include "KeyBindingStore.h"


using namespace Framework::Input::KeyBindings;



KeyBindingStore::KeyBindingStore(function<DatabaseContext*(void)> gContext, Storage * s): DatabaseStore(gContext, s)
{
	// ���ӳ��|�Ψ�
}
