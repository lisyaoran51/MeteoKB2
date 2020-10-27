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
		/// ���ӷ|�h���command���S���bstate���A�o���i�o��F�������handleNewBluetoothCommand�h
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, InputState* state, BluetoothCommand* command) {

			return InputManager::propagateBluetoothCommand(queue, state, command) + handleNewBluetoothCommand(queue, command);
		}

		/// <summary>
		/// �|��queue�̭���keybindingReceivable��X�ӡA�M��I�sonKeyDown(pair<T, int> key)
		/// �n�B�O�u�ݭn��key�A���ά�input state�A���²��ާ@
		/// </summary>
		virtual int propagateBluetoothCommand(vector<Triggerable*>* queue, TCommand* command) {

			LOG(LogLevel::Fine) << "CommandInputManager::propagateBluetoothCommand() : check if there is [" << queue->size() << "] command handler.";

			for (int i = 0; i < queue->size(); i++) {

				CommandHandler<TCommand>* commandHandler = dynamic_cast<CommandHandler<TCommand>*>(queue->at(i));
				if (commandHandler != nullptr) {

					LOG(LogLevel::Fine) << "CommandInputManager::propagateBluetoothCommand() : get command handler [" << queue->at(i)->GetTypeName() << "].";

					commandHandler->OnCommand(command);
				}
			}

			return 0;
		}

	private:

		int handleNewBluetoothCommand(vector<Triggerable*>* queue, BluetoothCommand* command) {

			if (dynamic_cast<TCommand*>(command) != nullptr) {

				LOG(LogLevel::Fine) << "CommandInputManager::handleNewBluetoothCommand() : get new T command.";
				propagateBluetoothCommand(queue, dynamic_cast<TCommand*>(command));
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