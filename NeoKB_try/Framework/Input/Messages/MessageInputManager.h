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
		/// ���ӷ|�h���command���S���bstate���A�o���i�o��F�������handleNewBluetoothCommand�h
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, InputState* state, BluetoothCommand* command) {

			return InputManager::propagateBluetoothCommand(queue, state, command) + handleNewBluetoothCommand(queue, command);
		}

		/// <summary>
		/// �|��queue�̭���keybindingReceivable��X�ӡA�M��I�sonKeyDown(pair<T, int> key)
		/// �n�B�O�u�ݭn��key�A���ά�input state�A���²��ާ@
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, TMessage* command) {

			LOG(LogLevel::Fine) << "MessageInputManager::propagateBluetoothCommand() : check if there is [" << queue->size() << "] command handler.";

			for (int i = 0; i < queue->size(); i++) {

				MessageHandler<TMessage>* commandHandler = dynamic_cast<MessageHandler<TMessage>*>(queue->at(i));
				if (commandHandler != nullptr) {

					LOG(LogLevel::Fine) << "MessageInputManager::propagateBluetoothCommand() : get command handler [" << queue->at(i)->GetTypeName() << "].";

					commandHandler->OnMessage(command);
				}
			}

			return 0;
		}

	private:

		int handleNewBluetoothCommand(vector<Triggerable*>* queue, BluetoothCommand* command) {

			if (dynamic_cast<TMessage*>(command) != nullptr) {

				LOG(LogLevel::Fine) << "MessageInputManager::handleNewBluetoothCommand() : get new T command.";
				propagateBluetoothCommand(queue, dynamic_cast<TMessage*>(command));
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