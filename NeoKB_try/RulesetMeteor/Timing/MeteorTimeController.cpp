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
	if (speedAdjuster->GetIsAdjustingTime())
		return -1;

	if (tempSection + 1 < repeatSections) {
		tempSection++;
		return 0;
	}

	tempSection = 0;

	if (section + repeatSections == sectionTime.size())	// �N��㭺�q�w�g���m���F
		return 0;

	float totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//�B�~�h�@��w�Įɶ�


	if (tempRepeatTimes < repeatTimes) {

		/*
		 * �o�䦳�ӫܤj�����D�A�N�O�b�˰h���ɭԡA�O���٬O�|�b�A�ѨM��k�O1.����O�������A�˰h���b���} 2.�����˰h�A���j�a�ݤ��� 3.�����V�W�O��
		 */
		if (repeatPracticeMode == RepeatPracticeMode::Demonstrate) {
			eventProcessorFilter->SwitchVariant(0);	// ���U�O���ܽd
			repeatPracticeMode == RepeatPracticeMode::Practice;
		}
		else {
			eventProcessorFilter->SwitchVariant(1);	// �V�W�O���m��
			repeatPracticeMode == RepeatPracticeMode::Demonstrate;
			tempRepeatTimes++;
		}
	}
	else {
		// �o�Ӭq���w�g�m���A�}�l�m�U�@�Ӭq��
		tempStartSection++;
		totalRewindLength = sectionTime[section + 1] - sectionTime[tempStartSection] + repeatBufferTime;	//�B�~�h�@��w�Įɶ�
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
			//SetAllChildsIsMaskedForTrigger(); �o��bspeed adjuster��on freeze�̭�
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

		speedAdjuster->SetSeekTime(turnValue * defaultAdjustTime);

		// �o��O�קK��nset seek time�H��seek time��n����0�A�|�y���N�~���p�A�ҥH��N�����L�̫��ܦ�0�A�N�H�K�[�@�ӼƦr�W�h
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