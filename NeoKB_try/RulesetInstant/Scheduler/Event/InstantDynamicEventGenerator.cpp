#include "InstantDynamicEventGenerator.h"

#include "Effect/InstantFallEffect.h"
#include "Effect/InstantGlowLineEffect.h"
#include "InstrumentEvents/InstantPianoSoundEvent.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::InstrumentEvents;
using namespace Games::Output::Bluetooths;



InstantDynamicEventGenerator::InstantDynamicEventGenerator(Playfield * p) : RegisterType("InstantDynamicEventGenerator"), DynamicEventGenerator(p)
{
}

int InstantDynamicEventGenerator::OnMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::InstantLightEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();

		if (context["EventType"].get<string>() == "Fall") {
			
			double lifeTime = context["Time"].get<double>();

			int speed = lifeTime * 16;

			int key = context["Key"].get<int>();

			double startTime = GetClock()->GetCurrentTime();

			// 如果是黑鍵，就要稍微往後移一點，不然會提早掉下來
			switch (key % 12) {
			case 1:
			case 3:
			case 6:
			case 8:
			case 10:
				startTime += lifeTime * 5.0 / 16.0;
				break;
			}

			InstantFallEffect* instantFallEffect = new InstantFallEffect(key, 0, startTime, lifeTime, speed);

			playfield->AddDynamic(instantFallEffect);
		}
		else if (context["EventType"].get<string>() == "Line") {

			double lifeTime = context["Time"].get<double>();

			int key = context["Key"].get<int>();

			double startTime = GetClock()->GetCurrentTime();

			InstantGlowLineEffect* instantGlowLineEffect = new InstantGlowLineEffect(key, 0, startTime, lifeTime);

			playfield->AddDynamic(instantGlowLineEffect);
		}

	}

	if (message->GetCommand() == MeteoCommand::InstantLedMatrix) {
		// 無法避免重疊問題?
		// 懶得做這個功能
	}

	if (message->GetCommand() == MeteoCommand::InstantPianoEvent) {

		MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);
		json context = contextMessage->GetContextInJson();

		double delayStartTime = context["DelayTime"].get<double>();

		int key = context["Key"].get<int>();

		int volume = context["Volume"].get<int>();

		double startTime = GetClock()->GetCurrentTime();

		InstantPianoSoundEvent* instantPianoSoundEvent = nullptr;

		if (key == -1)
			instantPianoSoundEvent = new InstantPianoSoundEvent(volume == 0, startTime + delayStartTime, 0);
		else
			instantPianoSoundEvent = new InstantPianoSoundEvent(pair<Pitch, float>((Pitch)key, float(volume) / 128.f), startTime + delayStartTime, 0);

		playfield->AddDynamic(instantPianoSoundEvent);

	}


	return 0;
}
