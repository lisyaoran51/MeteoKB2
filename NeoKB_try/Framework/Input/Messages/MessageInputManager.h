#ifndef MESSAGE_INPUT_MANAGER_H
#define MESSAGE_INPUT_MANAGER_H

#include "../KeyBindings/KeyBindingInputManager.h"
#include <utility>
#include "MessageHandler.h"
#include <algorithm>
#include <vector>


using namespace Framework::Input;
using namespace Framework::Input::KeyBindings;
using namespace std;



namespace Framework {
namespace Input {
namespace Messages {

	template<typename T, typename TMessage>
	class MessageInputManager : public KeyBindingInputManager<T> {


	public:

		MessageInputManager() : KeyBindingInputManager<T>(), RegisterType("MessageInputManager"){
			
		}

		virtual vector<KeyBinding*>* GetDefaultkeyBindings() {
			return new vector<KeyBinding*>();
		}

	protected:

		int command;

		/// <summary>
		/// 本來會去比對command有沒有在state李，這邊懶得比了直接丟到handleNewBluetoothCommand去
		/// </summary>
		virtual int propagateBluetoothMessage(vector<Triggerable*>* queue, InputState* state, BluetoothMessage* message) {

			return InputManager::propagateBluetoothMessage(queue, state, message) + handleNewBluetoothMessage(queue, message);
		}

		/// <summary>
		/// 會把queue裡面的keybindingReceivable抓出來，然後呼叫onKeyDown(pair<T, int> key)
		/// 好處是只需要抓key，不用看input state，比較簡單操作
		/// </summary>
		virtual int propagateBluetoothMessage(vector<Triggerable*>* queue, TMessage* message) {

			LOG(LogLevel::Fine) << "MessageInputManager::propagateBluetoothCommand() : check if there is [" << queue->size() << "] command handler.";

			for (int i = 0; i < queue->size(); i++) {

				MessageHandler<TMessage>* commandHandler = dynamic_cast<MessageHandler<TMessage>*>(queue->at(i));
				if (commandHandler != nullptr) {

					LOG(LogLevel::Fine) << "MessageInputManager::propagateBluetoothCommand() : get command handler [" << queue->at(i)->GetTypeName() << "].";

					commandHandler->OnMessage(message);
				}
			}

			return 0;
		}

	private:

		int handleNewBluetoothMessage(vector<Triggerable*>* queue, BluetoothMessage* message) {

			if (dynamic_cast<TMessage*>(message) != nullptr) {

				LOG(LogLevel::Fine) << "MessageInputManager::handleNewBluetoothCommand() : get new T command.";
				propagateBluetoothMessage(queue, dynamic_cast<TMessage*>(message));
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