#include "HandDifficultyModifier.h"

#include "../../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"
#include "../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"


using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Schedulers::Events::InstrumentEvents;



HandDifficultyModifier::HandDifficultyModifier() : RegisterType("HandDifficultyModifier")
{
}

HandDifficultyModifier::HandDifficultyModifier(SmDifficultyHandType hType, SmDifficultyDifficulty d) : RegisterType("HandDifficultyModifier")
{
	handType = hType;
	difficulty = d;
}

int HandDifficultyModifier::ApplyToDifficulty(SmDifficulty * smDifficulty)
{
	return 0;
}

int HandDifficultyModifier::ApplyToEventProcessorFilter(EventProcessorFilter * eventProcessorFilter)
{
	return 0;
}

int HandDifficultyModifier::SetValue(int value1, int value2)
{
	handType = (SmDifficultyHandType)value1;
	difficulty = (SmDifficultyDifficulty)value2;
	return 0;
}

SmDifficultyHandType HandDifficultyModifier::GetHandType()
{
	return SmDifficultyHandType();
}

SmDifficultyDifficulty HandDifficultyModifier::GetDifficulty()
{
	return SmDifficultyDifficulty();
}

bool HandDifficultyModifier::filterEventProcessorsByHandTypeAndDifficulty(EventProcessor<Event>* eventProcessor)
{
	Event* sourceEvent = eventProcessor->GetEvent()->GetSourceEvent();

	Event* eventToFilter = sourceEvent == nullptr ? eventProcessor->GetEvent() : sourceEvent;

	bool isInstrumentEvent = dynamic_cast<InstrumentEvent*>(eventProcessor->GetEvent()) != nullptr ? true : false;

	if (!eventToFilter)
		return true;

	PlayableControlPoint* playableControlPoint = dynamic_cast<PlayableControlPoint*>(eventToFilter);


	if (!playableControlPoint)
		return true;
	
	/* 單一譜檔 */
	if (!isInstrumentEvent) {
		switch (difficulty) {
		case SmDifficultyDifficulty::Easy:
			if (playableControlPoint->GetHandType() == HandType::LeftOther ||
				playableControlPoint->GetHandType() == HandType::RightOther) {
				return false;
			}
			break;

		}

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

	/* 複合式譜檔 */
	if (handType == SmDifficultyHandType::Left && difficulty == SmDifficultyDifficulty::Easy) {
		if (playableControlPoint->GetHandType() == HandType::LeftEasy ||
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			playableControlPoint->GetHandType() == HandType::LeftEasy || 
			)
	}


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

	return true;
}
