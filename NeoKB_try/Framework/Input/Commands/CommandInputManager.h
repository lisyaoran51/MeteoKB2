#ifndef COMMAND_INPUT_MANAGER_H
#define COMMAND_INPUT_MANAGER_H

#include "../KeyBindings/KeyBindingInputManager.h"
#include <utility>
#include "CommandHandler.h"
#include <algorithm>
#include <vector>


using namespace Framework::Input;
using namespace Framework::Input::KeyBindings;
using namespace std;



namespace Framework {
namespace Input {
namespace Commands {

	template<typename T, typename TCommand>
	class CommandInputManager : public KeyBindingInputManager<T> {


	public:

		CommandInputManager() : KeyBindingInputManager<T>(), RegisterType("CommandInputManager"){
			
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			return new vector<KeyBinding*>();
		}

	protected:

		int command;

		/// <summary>
		/// 本來會去比對command有沒有在state李，這邊懶得比了直接丟到handleNewBluetoothCommand去
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, InputState* state, BluetoothCommand* command) {

			return InputManager::propagateBluetoothCommand(queue, state, command) + handleNewBluetoothCommand(queue, command);
		}

		/// <summary>
		/// 會把queue裡面的keybindingReceivable抓出來，然後呼叫onKeyDown(pair<T, int> key)
		/// 好處是只需要抓key，不用看input state，比較簡單操作
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, TCommand* command) {

			for (int i = 0; i < queue->size(); i++) {
				CommandHandler<TCommand>* commandHandler = dynamic_cast<CommandHandler<TCommand>*>(queue->at(i));
				if (commandHandler != nullptr) {
					commandHandler->OnBluetoothCommand(command);
				}
			}

			return 0;
		}

	private:

		int handleNewBluetoothCommand(vector<Triggerable*>* queue, BluetoothCommand command) {

			if (dynamic_cast<TCommand*>(command) != nullptr) {
				propagateBluetoothCommand(queue, dynamic_cast<T*>(command));
				return 0;
			}
			else {
				return -1;
			}

			return 0;
		}



	};

}}}





#endif