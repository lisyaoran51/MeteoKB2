#include "MeteorSheetmusicPostProcessor.h"

#include "../Scheduler/Event/IoEvents/SustainPedalIoEvent.h"


using namespace Meteor::Config;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Schedulers::Events::IoEvents;


MeteorSmPostprocessor::MeteorSmPostprocessor() 
{
}

Sm<Event>* MeteorSmPostprocessor::postprocess(Sm<Event>* s)
{
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });

	SustainPedalIoEvent* lastSustainPedalIoEvent = nullptr;

	for (int i = 0; i < s->GetEvents()->size(); i++) {

		if (dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i))) {

			SustainPedalIoEvent* sustainPedalIoEvent = dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i));
			SustainPedalIoEvent* nextSustainPedalIoEvent = nullptr;

			for (int j = i; j < s->GetEvents()->size(); j++) {
				if (dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(j))) {
					nextSustainPedalIoEvent = dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(j));
					break;
				}
			}

			// 如果有下一個pedal，就以下一個pedal的時間為ringLifeTime。但有時候next target不是剛好接在這個pedal之後，中間有間隔一段時間
			// 那就不要管下一個pedal是多久以後，直接以現在的pedal踏完的時間做為ringLifeTime，
			MTO_FLOAT ringLifeTime = nextSustainPedalIoEvent == nullptr ? 
				sustainPedalIoEvent->GetLifeTime() : nextSustainPedalIoEvent->GetStartTime() - sustainPedalIoEvent->GetStartTime();

			if (ringLifeTime > sustainPedalIoEvent->GetLifeTime() + defaultNextPedalTargetStartTime * 2.f)
				ringLifeTime = sustainPedalIoEvent->GetLifeTime();

			/* 第一個pedal時 */
			if (lastSustainPedalIoEvent == nullptr) {

				sustainPedalIoEvent->SetTargetTime(
					sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime,
					defaultNextPedalTargetStartTime,
					sustainPedalIoEvent->GetLifeTime() - pedalInterval,
					ringLifeTime
				);

				

			}
			/* 第2~n個pedal */
			else {

				// 如果上一個pedal沒有跟這個連續的話，那就不要把target start time設在上一個pedal結束的時間，改成設在這個pedal快開始的時候。
				MTO_FLOAT targetStartTime = lastSustainPedalIoEvent->GetStartTime();
				MTO_FLOAT targetLifeTime = lastSustainPedalIoEvent->GetLifeTime();
				if (targetStartTime + lastSustainPedalIoEvent->GetLifeTime() < sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime * 2.f) {
					targetStartTime = sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime;
					targetLifeTime = defaultNextPedalTargetStartTime;
				}



				sustainPedalIoEvent->SetTargetTime(
					targetStartTime,
					targetLifeTime,
					sustainPedalIoEvent->GetLifeTime() - pedalInterval,
					ringLifeTime
				);

			}
			LOG(LogLevel::Debug) << "MeteorSmPostprocessor::postprocess() : sustainPedalIoEvent: [" << sustainPedalIoEvent->GetTargetStartTime() << "," << sustainPedalIoEvent->GetTargetLifeTime()
				<< "," << sustainPedalIoEvent->GetPedalDownLifeTime() << "," << sustainPedalIoEvent->GetRingLifeTime() << "]";
		}

	}


	return SmPostprocessor::postprocess(s);
}
