#ifndef KEY_BINDING_STORE_H
#define KEY_BINDING_STORE_H


#include "../../Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Framework {
namespace Input{
namespace KeyBindings{

	class KeyBindingStore : public DatabaseStore {


	public:

		KeyBindingStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);


	};

}}}




#endif