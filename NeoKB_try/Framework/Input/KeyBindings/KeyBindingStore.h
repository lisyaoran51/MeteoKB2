#ifndef KEY_BINDING_STORE_H
#define KEY_BINDING_STORE_H


#include "../../Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Framework {
namespace Input{
namespace KeyBindings{

	/// <summary>
	/// �o�����ӳ��|�Ψ�A�]��keybinding���Oinput manager�ۤv�]�w�n��
	/// </summary>
	class KeyBindingStore : public DatabaseStore {


	public:

		KeyBindingStore(function<DatabaseContext*(void)> gContext, Storage* s = nullptr);


	};

}}}




#endif