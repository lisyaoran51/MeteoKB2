#ifndef METEO_TIME_CONTROLLER_H
#define METEO_TIME_CONTROLLER_H


#include "../../Framework/Timing/TimeController.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Framework/Output/OutputManager.h"


using namespace Framework::Timing;
using namespace Games::Output::Bluetooths;
using namespace Framework::Output;


namespace Games {
namespace Timing{


	template<typename T>
	class MeteoTimeController : public TTimeController<T, MeteoBluetoothMessage> {

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

		

	protected:

		OutputManager* outputManager = nullptr;

		virtual int onMessage(MeteoBluetoothMessage* message) {

			if (message->GetCommand() == MeteoCommand::AppQuitGame) {

				LOG(LogLevel::Info) << "MeteoTimeController::onMessage() : get quit game message.";

				// 回傳Ack
				MeteoContextBluetoothMessage* ack = new MeteoContextBluetoothMessage(MeteoCommand::AckAppQuitGame);
				ack->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
				outputManager->PushMessage(ack);

				TimeController::onQuitRequested.Trigger();
			}
			else if (message->GetCommand() == MeteoCommand::AppRestartGame) {

				LOG(LogLevel::Info) << "MeteoTimeController::onMessage() : get restart game message.";

				// 回傳Ack
				MeteoContextBluetoothMessage* ack = new MeteoContextBluetoothMessage(MeteoCommand::AckAppRestartGame);
				ack->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
				outputManager->PushMessage(ack);

				TimeController::onRetryRequested.Trigger();
			}
			else if (message->GetCommand() == MeteoCommand::AppCompleteGame) {

				LOG(LogLevel::Info) << "MeteoTimeController::onMessage() : get complete game message.";

				// 回傳Ack
				MeteoContextBluetoothMessage* ack = new MeteoContextBluetoothMessage(MeteoCommand::AckAppCompleteGame);
				ack->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
				outputManager->PushMessage(ack);

				TimeController::onGameOver.Trigger();
			}

			return 0;
		}

	};


}}





#endif