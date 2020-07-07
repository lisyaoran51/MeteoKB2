#include "MeteorTimeController.h"

#include "../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"
#include "../Scheduler/Event/Effect/FallEffectMapper.h"
#include "../Scheduler/Event/Effect/EruptEffectMapper.h"



using namespace Meteor::Timing;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;



int MeteorTimeController::load()
{

	EventProcessorFilter * e = GetCache<EventProcessorFilter>("EventProcessorFilter");
	if (!e)
		throw runtime_error("MeteorTimeController::load() : EventProcessorFilter not found in cache.");

	return load(e);
}

int MeteorTimeController::load(EventProcessorFilter * eProcessorFilter)
{
	eventProcessorFilter = eProcessorFilter;
	return 0;
}

int MeteorTimeController::filterEventBySection(vector<EventProcessor<Event>*>* eventProcessors)
{

	for (int i = 0; i < eventProcessors->size(); i++) {
		PlayableControlPoint* controlPoint = dynamic_cast<PlayableControlPoint*>(eventProcessors->at(i)->GetEvent()->GetSourceEvent());
		if (controlPoint) {
			if (controlPoint->GetSectionIndex() < tempStartSection || controlPoint->GetSectionIndex() >= tempStartSection + repeatSections) {
				eventProcessors->erase(eventProcessors->begin() + i);
				i--;
			}
		}
	}

	return 0;
}

int MeteorTimeController::filterEruptEffect(vector<EventProcessor<Event>*>* eventProcessors)
{
	for (int i = 0; i < eventProcessors->size(); i++) {


	}


	return 0;
}

int MeteorTimeController::filterFallEffect(vector<EventProcessor<Event>*>* eventProcessors)
{
	return 0;
}

MeteorTimeController::MeteorTimeController() : RegisterType("MeteorTimeController")
{
	registerLoad(bind(static_cast<int(MeteorTimeController::*)(void)>(&MeteorTimeController::load), this));
}

int MeteorTimeController::SetSectionTime(vector<float>* sTime)
{
	for (int i = 0; i < sTime->size(); i++) {
		sectionTime.push_back(sTime->at(i));
	}
	return 0;
}

int MeteorTimeController::SetTimeControllerMode(MeteorTimeControllerMode tControllerMode)
{

	timeControllerMode = tControllerMode;

	if (tControllerMode == MeteorTimeControllerMode::RepeatPractice) {

		eventProcessorFilter->AddFilterCallback(bind(&MeteorTimeController::filterEventBySection, this, placeholders::_1));

		// 0是示範模式
		eventProcessorFilter->AddVariantFilterCallback(bind(&MeteorTimeController::filterEruptEffect, this, placeholders::_1), 0);

		// 1是練習模式
		eventProcessorFilter->AddVariantFilterCallback(bind(&MeteorTimeController::filterFallEffect, this, placeholders::_1), 1);

	}


	return 0;
}

int MeteorTimeController::SetRepeatSections(int rSections)
{
	repeatSections = rSections;

	return 0;
}

int MeteorTimeController::SetRepeatTimes(int rTimes)
{
	repeatTimes = rTimes;
	return 0;
}

int MeteorTimeController::SetHasSection(bool hSection)
{

	hasSection = hSection;
	return 0;
}

int MeteorTimeController::RepeatSection(int section)
{
	if (speedAdjuster->GetIsAdjustingTime())
		return -1;

	if (tempSection + 1 < repeatSections) {
		tempSection++;
		return 0;
	}

	tempSection = 0;

	if (section + repeatSections == sectionTime.size())	// 代表整首歌已經都練完了
		return 0;

	float totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//額外多一秒緩衝時間


	if (tempRepeatTimes < repeatTimes) {

		/*
		 * 這邊有個很大的問題，就是在倒退的時候，燈光還是會在，解決方法是1.先把燈光關掉，倒退完在打開 2.瞬間倒退，讓大家看不到 3.取消向上燈光
		 */
		if (repeatPracticeMode == RepeatPracticeMode::Demonstrate) {
			eventProcessorFilter->SwitchVariant(0);	// 落下燈光示範
			repeatPracticeMode == RepeatPracticeMode::Practice;
		}
		else {
			eventProcessorFilter->SwitchVariant(1);	// 向上燈光練習
			repeatPracticeMode == RepeatPracticeMode::Demonstrate;
			tempRepeatTimes++;
		}
	}
	else {
		// 這個段落已經練完，開始練下一個段落
		tempStartSection++;
		totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//額外多一秒緩衝時間
	}

	Pause();

	speedAdjuster->SetSeekTime(-totalRewindLength);

	return 0;
}

int MeteorTimeController::onButtonDown(InputState * inputState, InputKey button)
{
	if (button == InputKey::Pause) {
		LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : get pause button input and pause.";
		if (speedAdjuster->GetIsAdjustingTime())
			return -1;

		if (!GetIsPaused()) {
			Pause();
			//SetAllChildsIsMaskedForTrigger(); 這行在speed adjuster的on freeze裡面
		}
		else if (!isWaitingFreeze) {
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : restart and freeze 1 sec.";

			speedAdjuster->SetFreezeTime(defaultFreezeTime);
			isWaitingFreeze = true;
			isAdjustAfterPause = false;
		}
		else
			return -1;

	}
	return 0;
}

int MeteorTimeController::onKnobTurn(InputState * inputState, InputKey knob)
{
	if (knob == InputKey::SectionKnob) {
		// 倒數繼續遊戲的時候不准任何其他動作
		if (isWaitingFreeze)
			return 0;

		int turnValue = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetKnobs()->size(); i++)
			if (inputState->GetPanelState()->GetKnobs()->at(i).first == InputKey::SectionKnob)
				turnValue = inputState->GetPanelState()->GetKnobs()->at(i).second;

		if (!GetIsPaused()) {
			Pause();
		}
		else if(!speedAdjuster->GetIsAdjustingTime())
			isAdjustAfterPause = true;

		speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

		// 這邊是避免剛好set seek time以後seek time剛好等於0，會造成意外狀況，所以刻意不讓他最後變成0，就隨便加一個數字上去
		if(speedAdjuster->GetSeekTime() == 0)
			speedAdjuster->SetSeekTime(turnValue);

	}

	if (knob == InputKey::SpeedKnob) {
		LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : get [SpeedKnob] action. ";

		int turnValue = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetKnobs()->size(); i++)
			if (inputState->GetPanelState()->GetKnobs()->at(i).first == InputKey::SpeedKnob)
				turnValue = inputState->GetPanelState()->GetKnobs()->at(i).second;
		if (turnValue < 0) {
			if (GetRate() > 0.8) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				//SetRate(GetRate() - 0.1);
				SetRate(0.8);
			}
		}
		else {
			if (GetRate() < 1.5) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() + 0.1);

			}
		}


	}


	return 0;
}

/* 改成不繼承meteo action
map<MeteorAction, InputKey>* MeteorTimeController::GetDefaultkeyBindings()
{
	map<MeteorAction, InputKey>* tempMap = new map<MeteorAction, InputKey>();
	(*tempMap)[MeteorAction::Pause] = InputKey::Pause;
	(*tempMap)[MeteorAction::SectionKnob] = InputKey::SectionKnob;
	(*tempMap)[MeteorAction::SpeedKnob] = InputKey::SpeedKnob;
	(*tempMap)[MeteorAction::Bluetooth] = InputKey::Bluetooth;


	return tempMap;
}
*/

/* meteo action
int MeteorTimeController::reloadMappings()
{
	LOG(LogLevel::Info) << "MeteorTimeController::reloadMappings() : mapping key bindings.";
	map<MeteorAction, InputKey>* tempMap = GetDefaultkeyBindings();
	keyBindings = *tempMap;
	return 0;
}
*/