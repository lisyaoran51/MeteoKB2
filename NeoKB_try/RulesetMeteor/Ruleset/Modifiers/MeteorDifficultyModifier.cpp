#include "MeteorDifficultyModifier.h"

#include "../../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"
#include "../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Schedulers::Events::InstrumentEvents;



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

SmDifficultyDifficulty MeteorDifficultyModifier::GetDifficulty()
{
	return difficulty;
}

bool MeteorDifficultyModifier::filterEventProcessorsByDifficulty(EventProcessor<Event>* eventProcessor)
{
	Event* sourceEvent = eventProcessor->GetEvent()->GetSourceEvent();

	Event* eventToFilter = sourceEvent == nullptr ? eventProcessor->GetEvent() : sourceEvent;

	bool isInstrumentEvent = dynamic_cast<InstrumentEvent*>(eventProcessor->GetEvent()) != nullptr ? true : false;

	if (eventToFilter && !isInstrumentEvent) {
		PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(eventToFilter);
		if (playableControlPoint) {
			switch (difficulty) {
			case SmDifficultyDifficulty::Easy:
				if (playableControlPoint->GetHandType() == HandType::LeftOther ||
					playableControlPoint->GetHandType() == HandType::RightOther) {
					return false;
				}
				break;

			}
		}
	}

	return true;
}
