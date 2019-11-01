#ifndef KEY_BINDING_STORE_H
#define KEY_BINDING_STORE_H


#include "../../Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Framework {
namespace Input{
namespace KeyBindings{

	/// <summary>
	/// 這個應該部會用到，因為keybinding都是input manager自己設定好的
	/// </summary>
	class KeyBindingStore : public DatabaseStore {


	public:

		KeyBindingStore(function<DatabaseContext*(void)> gContext, Storage* s = nullptr);


	};

}}}




#endif