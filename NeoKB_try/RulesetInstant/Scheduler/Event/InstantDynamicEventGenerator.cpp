#include "InstantDynamicEventGenerator.h"

#include "Effect/InstantFallEffect.h"


using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;



int InstantDynamicEventGenerator::OnCommand(MeteoBluetoothCommand * command)
{
	if (command->GetCommand() == MeteoCommand::InstantLightEvent) {
		if (command->GetContext()["EventType"].get<string>() == "Fall") {
			
			double lifeTime = command->GetContext()["Time"].get<double>();

			int speed = lifeTime * 16;

			int key = command->GetContext()["Key"].get<int>();

			float startTime = GetClock()->GetCurrentTime();

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
	}


	return 0;
}
