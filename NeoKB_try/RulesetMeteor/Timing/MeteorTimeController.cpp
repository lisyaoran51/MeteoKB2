#include "MeteorTimeController.h"

#include "../../Games/Scheduler/Event/ControlPoints/PlayableControlPoint.h"
#include "../Scheduler/Event/Effect/FallEffectMapper.h"
#include "../Scheduler/Event/Effect/EruptEffectMapper.h"
#include "../Output/Panels/LightRingPanelMessage.h"
#include "../../Games/Output/Panels/SpeedRingPanelMessage.h"



using namespace Meteor::Timing;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Output::Panels;
using namespace Games::Output::Panels;



int MeteorTimeController::load()
{
	LOG(LogLevel::Info) << "MeteorTimeController::load() : loaded. ";
	
	return 0;
}

int MeteorTimeController::loadOnComplete(EventProcessorFilter * e)
{
	eventProcessorFilter = e;

	return 0;
}

bool MeteorTimeController::filterEventBySection(EventProcessor<Event>* eventProcessor)
{

	PlayableControlPoint* controlPoint = dynamic_cast<PlayableControlPoint*>(eventProcessor->GetEvent()->GetSourceEvent());
	if (controlPoint) {
		if (controlPoint->GetSectionIndex() < tempStartSection || controlPoint->GetSectionIndex() >= tempStartSection + repeatSections) {
			return false;
		}
	}


	return true;
}

bool MeteorTimeController::filterEruptEffect(EventProcessor<Event>* eventProcessor)
{
	EruptEffectMapper* eruptEffectMapper = dynamic_cast<EruptEffectMapper*>(eventProcessor);

	if (eruptEffectMapper) {
		return false;
	}

	


	return true;
}

bool MeteorTimeController::filterFallEffect(EventProcessor<Event>* eventProcessor)
{

	FallEffectMapper* fallEffectMapper = dynamic_cast<FallEffectMapper*>(eventProcessor);

	if (fallEffectMapper) {
		return false;
	}




	return true;
}

MeteorTimeController::MeteorTimeController() : RegisterType("MeteorTimeController")
{
	registerLoad(bind(static_cast<int(MeteorTimeController::*)(void)>(&MeteorTimeController::load), this));
}

int MeteorTimeController::SetRate(double r)
{
	MeteoTimeController<MeteorAction>::SetRate(r);

	SpeedRingPanelMessage* speedRingPanelMessage = nullptr;
	vector<bool> ringLights;

	/* 根據速度調整光圈 */
	if (rate >= 1.7) {
		//bool lightArray[6] = { true, true, true, true, true, false };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(4);
	}
	else if (rate >= 1.5) {
		//bool lightArray[6] = { true, true, true, true, false, false };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(3);
	}
	else if (rate >= 1.3) {
		//bool lightArray[6] = { true, true, true, false, false, false };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(2);
	}
	else if (rate >= 1.1) {
		//bool lightArray[6] = { true, true, false, false, false, false };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(1);
	}
	else if (rate >= 0.9) {	// 中央
		//bool lightArray[6] = { true, false, false, false, false, false };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(0);
	}
	else if (rate >= 0.7) {
		//bool lightArray[6] = { true, false, false, false, false, true };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(-1);
	}
	else if (rate >= 0.5) {
		//bool lightArray[6] = { true, false, false, false, true, true };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(-2);
	}
	else if (rate >= 0.3) {
		//bool lightArray[6] = { true, false, false, true, true, true };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(-3);
	}
	else if (rate >= 0.1) {
		//bool lightArray[6] = { true, false, true, true, true, true };
		//ringLights.assign(lightArray, lightArray + 6);
		speedRingPanelMessage = new SpeedRingPanelMessage(-4);
	}

	//speedRingPanelMessage = new SpeedRingPanelMessage(ringLights);
	outputManager->PushMessage(speedRingPanelMessage);

	return 0;
}

int MeteorTimeController::SetLastEventOverTime(double lEventOverTime)
{
	if (lEventOverTime > lastEventOverTime)
		lastEventOverTime = lEventOverTime;

	return 0;
}

int MeteorTimeController::OnButtonDown(MeteorAction action)
{
	if (action == MeteorAction::Pause) {
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

			/* 這編讓光圈跑一圈，跑的時間是defaultFreezeTime */
			LightRingPanelMessage* message = new LightRingPanelMessage(defaultFreezeTime);
			LOG(LogLevel::Depricated) << "MeteorTimeController::onButtonDown : send i2c [" << message->ToString() << "].";
			outputManager->PushMessage(message);
		}
		else
			return -1;

	}
	return 0;
}

int MeteorTimeController::OnKnobTurn(pair<MeteorAction, int> action)
{
	if (action.first == MeteorAction::SectionKnob) {
		// 倒數繼續遊戲的時候不准任何其他動作
		if (isWaitingFreeze)
			return 0;

		int turnValue = action.second;

		if (!GetIsPaused()) {
			Pause();
		}
		else if (!speedAdjuster->GetIsAdjustingTime())
			isAdjustAfterPause = true;

		if (timeControllerMode == MeteorTimeControllerMode::RepeatPractice) {
			if (turnValue > 0) {
				/* 往後轉的時候，就跳到下個小節 */
				speedAdjuster->SetSeekTime(GetClock()->GetCurrentTime() - sectionTime[tempStartSection + 1]);
				tempStartSection++;
			}
			else {
				/* 往回轉的時候，就跳到上個小節 */
				speedAdjuster->SetSeekTime(-(GetClock()->GetCurrentTime() - sectionTime[tempStartSection - 1]));
				tempStartSection--;
			}
			tempRepeatTimes = 0;
		}
		else if (timeControllerMode == MeteorTimeControllerMode::MusicGame) {

			/* 如果已經退到底，就不要再退 */
			if (controllableClock->GetCurrentTime() < 0 && turnValue < 0)
				return -1;

			speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

			// 這邊是避免剛好set seek time以後seek time剛好等於0，會造成意外狀況，所以刻意不讓他最後變成0，就隨便加一個數字上去
			if (speedAdjuster->GetSeekTime() == 0)
				speedAdjuster->SetSeekTime(turnValue);
		}



	}

	if (action.first == MeteorAction::SpeedKnob) {
		LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : get [SpeedKnob] action. ";

		int turnValue = action.second;

		if (turnValue < 0) {
			if (GetRate() > 0.3) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() - 0.1);
				//SetRate(0.8);

				// TODO:
				// OutputManager->push(減速訊號給mcu)

			}
		}
		else {
			if (GetRate() < 1.8) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() + 0.1);

				// TODO:
				// OutputManager->push(加速訊號給mcu)

			}
		}


	}


	return 0;
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
	LOG(LogLevel::Debug) << "MeteorTimeController::SetTimeControllerMode() : set to RepeatPractice.";
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
	/******************/
	repeatSections = 2;
	/******************/

	/* 低於repeatSections的前幾個小節不反覆 */
	if (section < repeatSections) {
		LOG(LogLevel::Debug) << 0;
		return -1;
	}

	if (speedAdjuster->GetIsAdjustingTime())
		return -1;

	/* 如果這個小節已經反覆過了，就不用再反覆 */
	if (section <= tempStartSection + repeatSections) {
		LOG(LogLevel::Debug) << 1;
		return -1;
	}

	//if (tempSection + 1 < repeatSections) 
	//{
	//	LOG(LogLevel::Debug) << 2;
	//	tempSection++;
	//	return 0;
	//} 

	// TODO: 切換Event processor filter
	float totalRewindLength = 0;

	if (tempRepeatTimes < repeatTimes) {

		// 這個段落已經練完，開始練下一個段落
		if (tempStartSection + repeatSections < section + 1) {
			

			if (tempRepeatTimes % 2 == 0) {
				eventProcessorFilter->SwitchVariant(0);	// 落下燈光示範
				repeatPracticeMode = RepeatPracticeMode::Demonstrate;
				LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : set filter to [" << 0 << "].";
			}
			else {
				eventProcessorFilter->SwitchVariant(1);	// 向上燈光練習
				repeatPracticeMode = RepeatPracticeMode::Practice;
				LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : set filter to [" << 1 << "].";
			}

			//if (repeatPracticeMode == RepeatPracticeMode::Demonstrate) {
			//	eventProcessorFilter->SwitchVariant(0);	// 落下燈光示範
			//	repeatPracticeMode == RepeatPracticeMode::Practice;
			//}
			//else {
			//	eventProcessorFilter->SwitchVariant(1);	// 向上燈光練習
			//	repeatPracticeMode == RepeatPracticeMode::Demonstrate;
			//	
			//}

		}

		//tempSection = 0;

		if (section + repeatSections == sectionTime.size())	// 代表整首歌已經都練完了
			return 0; 
		
		LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : jump to [" << section << "], temp section is [" << tempSection << "], repeat time is [" << tempRepeatTimes << "], tempStartSection is [" << tempStartSection << "].";


		totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//額外多一秒緩衝時間

		LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : total rewind length [" << totalRewindLength << "], section time [" << sectionTime[section + 1] << "], [" << sectionTime[tempStartSection] << "], section [" << section << "].";

		/* 這編讓光圈跑一圈，跑的時間是repeatBufferTime */
		LightRingPanelMessage* message = new LightRingPanelMessage(repeatBufferTime);
		LOG(LogLevel::Depricated) << "MeteorTimeController::RepeatSection : send i2c [" << message->ToString() << "].";
		outputManager->PushMessage(message);

		tempRepeatTimes++;
	}
	else {

		totalRewindLength = 0;
		tempStartSection++;

		if (tempStartSection + repeatSections < section + 1) {
			tempRepeatTimes = 0;
			RepeatSection(section);
		}
	}


	/*
	 * 這邊有個很大的問題，就是在倒退的時候，燈光還是會在，解決方法是1.先把燈光關掉，倒退完在打開 2.瞬間倒退，讓大家看不到 3.取消向上燈光
	 * 現在用方法2，直接跳過去
	 */

	if(totalRewindLength > 0)
		JumpTo(controllableClock->GetCurrentTime() - totalRewindLength);
	else
		LOG(LogLevel::Debug) << 3;


	

	return 0;
}

//int MeteorTimeController::update()
//{
//	//if (controllableClock->GetCurrentTime() > sectionTime[tempSection])
//	//	tempSection++;
//	return MeteoTimeController<MeteorAction>::update();
//}

int MeteorTimeController::LoadOnComplete()
{

	SpeedRingPanelMessage* speedRingPanelMessage = new SpeedRingPanelMessage(vector<bool>({true, false, false, false, false, false}));
	outputManager->PushMessage(speedRingPanelMessage);


	LOG(LogLevel::Debug) << "MeteorTimeController::LoadOnComplete() : loading event processor filter.";

	EventProcessorFilter * e = GetCache<EventProcessorFilter>("EventProcessorFilter");
	if (!e)
		throw runtime_error("MeteorTimeController::load() : EventProcessorFilter not found in cache.");



	return loadOnComplete(e);
}

bool MeteorTimeController::checkIsGameOver()
{
	// controllableClock 可能還沒started，就會跳錯誤

	try {
		if (controllableClock->GetCurrentTime() > lastEventOverTime + 3.0)
			return true;
		else
			return false;
	}
	catch (exception& e) {
		LOG(LogLevel::Fine) << "MeteorTimeController::checkIsGameOver() : clock [" << controllableClock << "] not started yet [" << e.what() << "].";
		return false;
	}
}

int MeteorTimeController::onButtonDown(InputState * inputState, InputKey button)
{
	return 0;

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

			/* 這編讓光圈跑一圈，跑的時間是defaultFreezeTime */
			LightRingPanelMessage* message = new LightRingPanelMessage(defaultFreezeTime);
			LOG(LogLevel::Depricated) << "MeteorTimeController::onButtonDown : send i2c [" << message->ToString() << "].";
			outputManager->PushMessage(message);
		}
		else
			return -1;

	}
	return 0;
}

int MeteorTimeController::onKnobTurn(InputState * inputState, InputKey knob)
{
	return 0;
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

		if (timeControllerMode == MeteorTimeControllerMode::RepeatPractice) {
			if (turnValue > 0) {
				/* 往後轉的時候，就跳到下個小節 */
				speedAdjuster->SetSeekTime(GetClock()->GetCurrentTime() - sectionTime[tempStartSection + 1]);
				tempStartSection++;
			}
			else {
				/* 往回轉的時候，就跳到上個小節 */
				speedAdjuster->SetSeekTime(-(GetClock()->GetCurrentTime() - sectionTime[tempStartSection - 1]));
				tempStartSection--;
			}
			tempRepeatTimes = 0;
		}
		else if(timeControllerMode == MeteorTimeControllerMode::MusicGame){

			speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

			// 這邊是避免剛好set seek time以後seek time剛好等於0，會造成意外狀況，所以刻意不讓他最後變成0，就隨便加一個數字上去
			if (speedAdjuster->GetSeekTime() == 0)
				speedAdjuster->SetSeekTime(turnValue);
		}
		


	}

	if (knob == InputKey::SpeedKnob) {
		LOG(LogLevel::Depricated) << "MeteorTimeController::onKnobTurn() : get [SpeedKnob] action. ";

		int turnValue = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetKnobs()->size(); i++)
			if (inputState->GetPanelState()->GetKnobs()->at(i).first == InputKey::SpeedKnob)
				turnValue = inputState->GetPanelState()->GetKnobs()->at(i).second;
		if (turnValue < 0) {
			if (GetRate() > 0.3) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() - 0.1);

				//SetRate(0.8);

				// TODO:
				// OutputManager->push(減速訊號給mcu)

			}
		}
		else {
			if (GetRate() < 1.8) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() + 0.1);

				// TODO:
				// OutputManager->push(加速訊號給mcu)

			}
		}


	}


	return 0;
}

int MeteorTimeController::onMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::AppQuitGame || message->GetCommand() == MeteoCommand::AppRestartGame) {

		/* 離開遊戲時將燈光關閉 */
		SpeedRingPanelMessage* speedRingPanelMessage = new SpeedRingPanelMessage(5);
		outputManager->PushMessage(speedRingPanelMessage);

	}

	return MeteoTimeController<MeteorAction>::onMessage(message);
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