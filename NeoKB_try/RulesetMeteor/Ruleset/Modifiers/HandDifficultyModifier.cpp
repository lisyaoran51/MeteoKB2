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
	smDifficulty->HandType = handType;
	smDifficulty->Difficulty = difficulty;

	return 0;
}

int HandDifficultyModifier::ApplyToEventProcessorFilter(EventProcessorFilter * eventProcessorFilter)
{
	eventProcessorFilter->AddFilterCallback(bind(&HandDifficultyModifier::filterEventProcessorsByHandTypeAndDifficulty, this, placeholders::_1));
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

		if (playableControlPoint->GetHandType() == HandType::Hidden)
			return false;
	}

	/* 複合式譜檔 */
	if (handType == SmDifficultyHandType::Left && difficulty == SmDifficultyDifficulty::None) {			// 純左手
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::LeftOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::LeftOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::LeftOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::LeftOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::Right && difficulty == SmDifficultyDifficulty::None) {	// 純右手
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::RightOnly &&
			playableControlPoint->GetHandType() != HandType::RightOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::RightOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::RightOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::RightOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::None && difficulty == SmDifficultyDifficulty::Easy) {	// 純簡單
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::EasyOnly &&
			playableControlPoint->GetHandType() != HandType::EasyOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::EasyOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::EasyOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::EasyOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::None && difficulty == SmDifficultyDifficulty::Hard) {	// 純困難
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::HardOnly &&
			playableControlPoint->GetHandType() != HandType::HardOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::HardOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::HardOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::HardOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::Left && difficulty == SmDifficultyDifficulty::Easy) {	// 純左手簡單
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::LeftEasyOnly &&
			playableControlPoint->GetHandType() != HandType::LeftEasyOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::LeftEasyOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::LeftEasyOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::LeftEasyOnlyHidden && !isInstrumentEvent)	
			return false;
	}
	else if (handType == SmDifficultyHandType::Left && difficulty == SmDifficultyDifficulty::Hard) {	// 純左手困難
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::LeftHardOnly &&
			playableControlPoint->GetHandType() != HandType::LeftHardOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::LeftHardOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::LeftHardOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::LeftHardOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::Right && difficulty == SmDifficultyDifficulty::Easy) {	// 純右手簡單
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::RightEasyOnly &&
			playableControlPoint->GetHandType() != HandType::RightEasyOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::RightEasyOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::RightEasyOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::RightEasyOnlyHidden && !isInstrumentEvent)
			return false;
	}
	else if (handType == SmDifficultyHandType::Right && difficulty == SmDifficultyDifficulty::Hard) {	// 純右手困難
		if (playableControlPoint->GetHandType() >= HandType::LeftOnly &&
			playableControlPoint->GetHandType() != HandType::RightHardOnly &&
			playableControlPoint->GetHandType() != HandType::RightHardOnlyFoot &&
			playableControlPoint->GetHandType() != HandType::RightHardOnlyOctaveShift &&
			playableControlPoint->GetHandType() != HandType::RightHardOnlyHidden)
			return false;
		if (playableControlPoint->GetHandType() == HandType::RightHardOnlyHidden && !isInstrumentEvent)
			return false;
	}



	return true;
}
