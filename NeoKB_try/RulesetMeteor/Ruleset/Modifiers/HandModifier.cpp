#include "HandModifier.h"

#include "../../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"

using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;


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

int HandModifier::filterEventProcessorsByHandType(vector<EventProcessor<Event>*>* eventProcessors)
{
	for (int i = 0; i < eventProcessors->size(); i++) {

		Event* sourceEvent = eventProcessors->at(i)->GetEvent()->GetSourceEvent();

		Event* eventToFilter = sourceEvent == nullptr ? eventProcessors->at(i)->GetEvent() : sourceEvent;

		if (eventToFilter) {
			PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(eventToFilter);
			if (playableControlPoint) {
				switch (handType) {
				case SmDifficultyHandType::Left:
					if (playableControlPoint->GetHandType() == HandType::RightEasy ||
						playableControlPoint->GetHandType() == HandType::RightOther) {
						eventProcessors->erase(eventProcessors->begin() + i);
						i--;
					}
					break;

				case SmDifficultyHandType::Right:
					if (playableControlPoint->GetHandType() == HandType::LeftEasy ||
						playableControlPoint->GetHandType() == HandType::LeftOther) {
						eventProcessors->erase(eventProcessors->begin() + i);
						i--;
					}
					break;
				}
			}
		}


	}

	return 0;
}
