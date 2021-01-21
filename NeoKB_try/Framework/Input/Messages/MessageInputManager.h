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
		virtual int propagateBluetoothMessage(vector<Triggerable*>* queue, InputState* state, BluetoothMessage* message) {

			return InputManager::propagateBluetoothMessage(queue, state, message) + handleNewBluetoothMessage(queue, message);
		}

		/// <summary>
		/// �|��queue�̭���keybindingReceivable��X�ӡA�M��I�sonKeyDown(pair<T, int> key)
		/// �n�B�O�u�ݭn��key�A���ά�input state�A���²��ާ@
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