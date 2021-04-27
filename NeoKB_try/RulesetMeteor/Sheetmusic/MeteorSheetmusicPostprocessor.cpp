#include "MeteorSheetmusicPostProcessor.h"

#include "../Scheduler/Event/IoEvents/SustainPedalIoEvent.h"
#include "../Scheduler/Event/TimeEvents/RepeatPracticeEvent.h"


using namespace Meteor::Config;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Schedulers::Events::IoEvents;
using namespace Meteor::Schedulers::Events::TimeEvents;


MeteorSmPostprocessor::MeteorSmPostprocessor() 
{
}

Sm<Event>* MeteorSmPostprocessor::postprocess(Sm<Event>* s)
{
	LOG(LogLevel::Fine) << "MeteorSmPostprocessor::postprocess() : Start sorting ...";

	// true->a先，false->b先
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { 
		
		/* 如果有time event的話，一律time event優先 */
		if (a->GetStartTime() == b->GetStartTime()) {
			if (dynamic_cast<RepeatPracticeEvent*>(a)) {
				return true;
			}
			if (dynamic_cast<RepeatPracticeEvent*>(b)) {
				return false;
			}
		}
		
		return a->GetStartTime() < b->GetStartTime(); 
	
	});

	// sort(s->GetEvents()->begin(), s->GetEvents()->end());
	return s;




	// 不對光圈做後處理，以後就踏多久，光圈就亮多久，不用提早顯示，也不用管下一個光圈在多久以後

	return SmPostprocessor::postprocess(s);



	// --------------------------------------------舊的code--------------------------------------------

	/*
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });

	SustainPedalIoEvent* lastSustainPedalIoEvent = nullptr;

	for (int i = 0; i < s->GetEvents()->size(); i++) {

		if (dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i))) {

			SustainPedalIoEvent* sustainPedalIoEvent = dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i));
			SustainPedalIoEvent* nextSustainPedalIoEvent = nullptr;

			for (int j = i + 1; j < s->GetEvents()->size(); j++) {
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

			// 第一個pedal時 
			if (lastSustainPedalIoEvent == nullptr) {

				sustainPedalIoEvent->SetTargetTime(
					sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime,
					defaultNextPedalTargetStartTime,
					sustainPedalIoEvent->GetLifeTime() - pedalInterval,
					ringLifeTime
				);

				

			}
			// 第2~n個pedal 
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
			LOG(LogLevel::Depricated) << "MeteorSmPostprocessor::postprocess() : sustainPedalIoEvent: [" << sustainPedalIoEvent->GetTargetStartTime() << "," << sustainPedalIoEvent->GetTargetLifeTime()
				<< "," << sustainPedalIoEvent->GetPedalDownLifeTime() << "," << sustainPedalIoEvent->GetRingLifeTime() << "]";
			
			lastSustainPedalIoEvent = sustainPedalIoEvent;
		}

	}


	return SmPostprocessor::postprocess(s);
	*/
}
