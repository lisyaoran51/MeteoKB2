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

	/* �ھڳt�׽վ���� */
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
	else if (rate >= 0.9) {	// ����
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
			//SetAllChildsIsMaskedForTrigger(); �o��bspeed adjuster��on freeze�̭�
		}
		else if (!isWaitingFreeze) {
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : restart and freeze 1 sec.";

			speedAdjuster->SetFreezeTime(defaultFreezeTime);
			isWaitingFreeze = true;
			isAdjustAfterPause = false;

			/* �o�s������]�@��A�]���ɶ��OdefaultFreezeTime */
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
		// �˼��~��C�����ɭԤ�������L�ʧ@
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
				/* �����઺�ɭԡA�N����U�Ӥp�` */
				speedAdjuster->SetSeekTime(GetClock()->GetCurrentTime() - sectionTime[tempStartSection + 1]);
				tempStartSection++;
			}
			else {
				/* ���^�઺�ɭԡA�N����W�Ӥp�` */
				speedAdjuster->SetSeekTime(-(GetClock()->GetCurrentTime() - sectionTime[tempStartSection - 1]));
				tempStartSection--;
			}
			tempRepeatTimes = 0;
		}
		else if (timeControllerMode == MeteorTimeControllerMode::MusicGame) {

			/* �p�G�w�g�h�쩳�A�N���n�A�h */
			if (controllableClock->GetCurrentTime() < 0 && turnValue < 0)
				return -1;

			speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

			// �o��O�קK��nset seek time�H��seek time��n����0�A�|�y���N�~���p�A�ҥH��N�����L�̫��ܦ�0�A�N�H�K�[�@�ӼƦr�W�h
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
				// OutputManager->push(��t�T����mcu)

			}
		}
		else {
			if (GetRate() < 1.8) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() + 0.1);

				// TODO:
				// OutputManager->push(�[�t�T����mcu)

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

		// 0�O�ܽd�Ҧ�
		eventProcessorFilter->AddVariantFilterCallback(bind(&MeteorTimeController::filterEruptEffect, this, placeholders::_1), 0);

		// 1�O�m�߼Ҧ�
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

	/* �C��repeatSections���e�X�Ӥp�`������ */
	if (section < repeatSections) {
		LOG(LogLevel::Debug) << 0;
		return -1;
	}

	if (speedAdjuster->GetIsAdjustingTime())
		return -1;

	/* �p�G�o�Ӥp�`�w�g���йL�F�A�N���ΦA���� */
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

	// TODO: ����Event processor filter
	float totalRewindLength = 0;

	if (tempRepeatTimes < repeatTimes) {

		// �o�Ӭq���w�g�m���A�}�l�m�U�@�Ӭq��
		if (tempStartSection + repeatSections < section + 1) {
			

			if (tempRepeatTimes % 2 == 0) {
				eventProcessorFilter->SwitchVariant(0);	// ���U�O���ܽd
				repeatPracticeMode = RepeatPracticeMode::Demonstrate;
				LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : set filter to [" << 0 << "].";
			}
			else {
				eventProcessorFilter->SwitchVariant(1);	// �V�W�O���m��
				repeatPracticeMode = RepeatPracticeMode::Practice;
				LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : set filter to [" << 1 << "].";
			}

			//if (repeatPracticeMode == RepeatPracticeMode::Demonstrate) {
			//	eventProcessorFilter->SwitchVariant(0);	// ���U�O���ܽd
			//	repeatPracticeMode == RepeatPracticeMode::Practice;
			//}
			//else {
			//	eventProcessorFilter->SwitchVariant(1);	// �V�W�O���m��
			//	repeatPracticeMode == RepeatPracticeMode::Demonstrate;
			//	
			//}

		}

		//tempSection = 0;

		if (section + repeatSections == sectionTime.size())	// �N��㭺�q�w�g���m���F
			return 0; 
		
		LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : jump to [" << section << "], temp section is [" << tempSection << "], repeat time is [" << tempRepeatTimes << "], tempStartSection is [" << tempStartSection << "].";


		totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//�B�~�h�@��w�Įɶ�

		LOG(LogLevel::Debug) << "MeteorTimeController::RepeatSection() : total rewind length [" << totalRewindLength << "], section time [" << sectionTime[section + 1] << "], [" << sectionTime[tempStartSection] << "], section [" << section << "].";

		/* �o�s������]�@��A�]���ɶ��OrepeatBufferTime */
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
	 * �o�䦳�ӫܤj�����D�A�N�O�b�˰h���ɭԡA�O���٬O�|�b�A�ѨM��k�O1.����O�������A�˰h���b���} 2.�����˰h�A���j�a�ݤ��� 3.�����V�W�O��
	 * �{�b�Τ�k2�A�������L�h
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
	// controllableClock �i���٨Sstarted�A�N�|�����~

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
			//SetAllChildsIsMaskedForTrigger(); �o��bspeed adjuster��on freeze�̭�
		}
		else if (!isWaitingFreeze) {
			LOG(LogLevel::Debug) << "MeteorTimeController::OnButtonDown() : restart and freeze 1 sec.";

			speedAdjuster->SetFreezeTime(defaultFreezeTime);
			isWaitingFreeze = true;
			isAdjustAfterPause = false;

			/* �o�s������]�@��A�]���ɶ��OdefaultFreezeTime */
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
		// �˼��~��C�����ɭԤ�������L�ʧ@
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
				/* �����઺�ɭԡA�N����U�Ӥp�` */
				speedAdjuster->SetSeekTime(GetClock()->GetCurrentTime() - sectionTime[tempStartSection + 1]);
				tempStartSection++;
			}
			else {
				/* ���^�઺�ɭԡA�N����W�Ӥp�` */
				speedAdjuster->SetSeekTime(-(GetClock()->GetCurrentTime() - sectionTime[tempStartSection - 1]));
				tempStartSection--;
			}
			tempRepeatTimes = 0;
		}
		else if(timeControllerMode == MeteorTimeControllerMode::MusicGame){

			speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

			// �o��O�קK��nset seek time�H��seek time��n����0�A�|�y���N�~���p�A�ҥH��N�����L�̫��ܦ�0�A�N�H�K�[�@�ӼƦr�W�h
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
				// OutputManager->push(��t�T����mcu)

			}
		}
		else {
			if (GetRate() < 1.8) {
				LOG(LogLevel::Debug) << "MeteorTimeController::onKnobTurn() : [SpeedKnob] action turn value = " << turnValue << ". rate = " << GetRate();
				SetRate(GetRate() + 0.1);

				// TODO:
				// OutputManager->push(�[�t�T����mcu)

			}
		}


	}


	return 0;
}

int MeteorTimeController::onMessage(MeteoBluetoothMessage * message)
{
	if (message->GetCommand() == MeteoCommand::AppQuitGame || message->GetCommand() == MeteoCommand::AppRestartGame) {

		/* ���}�C���ɱN�O������ */
		SpeedRingPanelMessage* speedRingPanelMessage = new SpeedRingPanelMessage(5);
		outputManager->PushMessage(speedRingPanelMessage);

	}

	return MeteoTimeController<MeteorAction>::onMessage(message);
}

/* �令���~��meteo action
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