#include "MusicGameModifier.h"

#include "../../Scheduler/Event/Effect/EruptEffectMapper.h"
#include "../../Scheduler/Event/TimeEvents/RepeatPracticeEventProcessor.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::TimeEvents;





MusicGameModifier::MusicGameModifier() : RegisterType("MusicGameModifier")
{



}

int MusicGameModifier::ApplyToEventProcessorFilter(EventProcessorFilter * eventProcessorFilter)
{

	eventProcessorFilter->AddFilterCallback(bind(&MusicGameModifier::filterRepeatPracticeEventProcessors, this, placeholders::_1));


	eventProcessorFilter->AddFilterCallback(bind(&MusicGameModifier::filterEruptEffects, this, placeholders::_1));

	return 0;
}

int MusicGameModifier::filterRepeatPracticeEventProcessors(vector<EventProcessor<Event>*>* eventProcessors)
{

	for (int i = 0; i < eventProcessors->size(); i++) {

		RepeatPracticeEventProcessor* repeatPracticeEventProcessor = dynamic_cast<RepeatPracticeEventProcessor*>(eventProcessors->at(i));

		if (repeatPracticeEventProcessor) {
			eventProcessors->erase(eventProcessors->begin() + i);
			i--;
		}

	}

	return 0;
}

int MusicGameModifier::filterEruptEffects(vector<EventProcessor<Event>*>* eventProcessors)
{
	for (int i = 0; i < eventProcessors->size(); i++) {

		EruptEffectMapper* eruptEffectMapper = dynamic_cast<EruptEffectMapper*>(eventProcessors->at(i));

		if (eruptEffectMapper) {
			eventProcessors->erase(eventProcessors->begin() + i);
			i--;
		}

	}

	return 0;
}
