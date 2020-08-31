#include "MeteorDifficultyModifier.h"

#include "../../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;



MeteorDifficultyModifier::MeteorDifficultyModifier() :RegisterType("DifficultyModifier")
{
}

MeteorDifficultyModifier::MeteorDifficultyModifier(SmDifficultyDifficulty d) :RegisterType("DifficultyModifier")
{
	difficulty = d;
}

int MeteorDifficultyModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	smDifficulty->Difficulty = difficulty;

	return 0;
}

int MeteorDifficultyModifier::ApplyToEventProcessorFilter(EventProcessorFilter * eventProcessorFilter)
{

	eventProcessorFilter->AddFilterCallback(bind(&MeteorDifficultyModifier::filterEventProcessorsByDifficulty, this, placeholders::_1));


	return 0;
}

int MeteorDifficultyModifier::SetValue(int value1, int value2)
{
	difficulty = (SmDifficultyDifficulty)value1;
	return 0;
}

int MeteorDifficultyModifier::filterEventProcessorsByDifficulty(vector<EventProcessor<Event>*>* eventProcessors)
{
	for (int i = 0; i < eventProcessors->size(); i++) {

		Event* sourceEvent = eventProcessors->at(i)->GetEvent()->GetSourceEvent();

		Event* eventToFilter = sourceEvent == nullptr ? eventProcessors->at(i)->GetEvent() : sourceEvent;

		if (eventToFilter) {
			PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(eventToFilter);
			if (playableControlPoint) {
				switch (difficulty) {
				case SmDifficultyDifficulty::Easy:
					if (playableControlPoint->GetHandType() == HandType::LeftOther ||
						playableControlPoint->GetHandType() == HandType::RightOther) {
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
