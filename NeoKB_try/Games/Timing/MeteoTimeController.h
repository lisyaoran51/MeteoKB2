#ifndef METEO_TIME_CONTROLLER_H
#define METEO_TIME_CONTROLLER_H


#include "../../Framework/Timing/TimeController.h"
#include "../Input/Commands/MeteoBluetoothCommand.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Framework/Output/OutputManager.h"


using namespace Framework::Timing;
using namespace Games::Input::Commands;
using namespace Games::Output::Bluetooths;
using namespace Framework::Output;


namespace Games {
namespace Timing{


	template<typename T>
	class MeteoTimeController : public TTimeController<T, MeteoBluetoothCommand> {

		int load() {
			OutputManager * o = TimeController::GetCache<OutputManager>("OutputManager");
			if (!o)
				throw runtime_error("int MeteoTimeController::load() : OutputManager not found in cache.");
			
			return load(o);
		}

		int load(OutputManager* o) {
			outputManager = o;
			return 0;
		}


	public:

		MeteoTimeController() : RegisterType("MeteoTimeController") {
			TimeController::registerLoad(bind(static_cast<int(MeteoTimeController<T>::*)(void)>(&MeteoTimeController<T>::load), this));
		}

		virtual int OnCommand(MeteoBluetoothCommand* command) {

			if (command->GetCommand() == MeteoCommand::AppQuitGame) {

				// �^��Ack
				MeteoContextBluetoothMessage* ack = new MeteoContextBluetoothMessage(MeteoCommand::AckAppQuitGame);
				outputManager->PushMessage(ack);

				TimeController::onQuitRequested.Trigger();
			}
			else if (command->GetCommand() == MeteoCommand::AppRestartGame) {

				// �^��Ack
				MeteoContextBluetoothMessage* ack = new MeteoContextBluetoothMessage(MeteoCommand::AckAppRestartGame);
				outputManager->PushMessage(ack);

				TimeController::onRetryRequested.Trigger();
			}

			return 0;
		}

	protected:

		OutputManager* outputManager = nullptr;

	};


}}





#endif