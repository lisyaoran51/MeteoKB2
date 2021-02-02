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

bool MusicGameModifier::filterRepeatPracticeEventProcessors(EventProcessor<Event>* eventProcessor)
{
	if (eventProcessor->GetEventProcessorType() == EventProcessorType::Time) {

		RepeatPracticeEventProcessor* repeatPracticeEventProcessor = dynamic_cast<RepeatPracticeEventProcessor*>(eventProcessor);

		if (repeatPracticeEventProcessor) {
			return false;
		}

	}

	

	return true;
}

bool MusicGameModifier::filterEruptEffects(EventProcessor<Event>* eventProcessor)
{
	if (eventProcessor->GetEventProcessorType() == EventProcessorType::Effect) {

		EruptEffectMapper* eruptEffectMapper = dynamic_cast<EruptEffectMapper*>(eventProcessor);

		if (eruptEffectMapper) {
			return false;
		}
	}

	

	return true;
}
