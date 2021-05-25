#include "HandModifier.h"

#include "../../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"
#include "../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"

using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Schedulers::Events::InstrumentEvents;


HandModifier::HandModifier() : RegisterType("HandModifier")
{
}

HandModifier::HandModifier(SmDifficultyHandType hType) : RegisterType("HandModifier")
{
	handType = hType;
}

int HandModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->HandType = handType;
	return 0;
}

int HandModifier::ApplyToEventProcessorFilter(EventProcessorFilter * eventProcessorFilter)
{

	eventProcessorFilter->AddFilterCallback(bind(&HandModifier::filterEventProcessorsByHandType, this, placeholders::_1));

	return 0;
}

int HandModifier::SetValue(int value1, int value2)
{
	handType = (SmDifficultyHandType)value1;
	return 0;
}

SmDifficultyHandType HandModifier::GetHandType()
{
	return handType;
}

bool HandModifier::filterEventProcessorsByHandType(EventProcessor<Event>* eventProcessor)
{
	Event* sourceEvent = eventProcessor->GetEvent()->GetSourceEvent();

	Event* eventToFilter = sourceEvent == nullptr ? eventProcessor->GetEvent() : sourceEvent;

	bool isInstrumentEvent = dynamic_cast<InstrumentEvent*>(sourceEvent) != nullptr ? true : false;

	if (eventToFilter && !isInstrumentEvent) {
		PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(eventToFilter);
		if (playableControlPoint) {
			switch (handType) {
			case SmDifficultyHandType::Left:
				if (playableControlPoint->GetHandType() == HandType::RightEasy ||
					playableControlPoint->GetHandType() == HandType::RightOther) {
					return false;
				}
				break;

			case SmDifficultyHandType::Right:
				if (playableControlPoint->GetHandType() == HandType::LeftEasy ||
					playableControlPoint->GetHandType() == HandType::LeftOther) {
					return false;
				}
				break;
			}
		}
	}

	return true;
}
