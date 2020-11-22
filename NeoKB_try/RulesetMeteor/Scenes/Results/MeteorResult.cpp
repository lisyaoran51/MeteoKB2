#include "MeteorResult.h"

#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Output/Bluetooths/MeteoFileBluetoothMessage.h"
#include "../../Ruleset/Modifiers/HandModifier.h"
#include "../../Ruleset/Modifiers/MeteorDifficultyModifier.h"
#include "../../../Games/Scheduler/Event/ControlPoints/SectionStartControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../Input/MeteorPitchBindingSet.h"
#include "../../Ruleset/Replays/MeteorReplayFrame.h"


using namespace Meteor::Scenes::Results;
using namespace Games::Output::Bluetooths;
using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Meteor::Input;
using namespace Meteor::Rulesets::Replays;


int MeteorResult::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("MeteorResult::load() : OutputManager not found in cache.");

	Instrument * i = GetCache<Instrument>("Instrument");
	if (!i)
		throw runtime_error("MeteorResult::load() : Instrument not found in cache.");

	return load(o, i);
}

int MeteorResult::load(OutputManager * o, Instrument * i)
{
	outputManager = o;
	piano = dynamic_cast<Piano*>(i);

	if (piano == nullptr) {
		LOG(LogLevel::Error) << "MeteorResult::load : Instrument cannot cast to piano.";
	}

	return 0;
}

MeteorResult::MeteorResult(Score * s) : Result(s), RegisterType("MeteorResult")
{

	registerLoad(bind((int(MeteorResult::*)())&MeteorResult::load, this));
}

string MeteorResult::writeGameRecord()
{
	/* �O�_�ϥέ�l��O��T�A�p�G�и̥��ӴN����O��T�A�^�S�S����O�A�N�έ�l��O��T */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
								piano->GetSustainType() == SustainType::GameControllingSustain;

	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;

	/* �o���C���O�έ����� */
	SmDifficultyHandType hand = SmDifficultyHandType::None;

	/* �o���C�����׬��� */
	SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

	for (int i = 0; i < workingSm.GetValue()->GetModifiers()->GetValue()->size(); i++) {
		if (dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			hand = dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetHandType();
		}
		if (dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			difficulty = dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetDifficulty();
		}
	}

	/* �ΨӰO���ثe�b���@�ӤK�� */
	vector<pair<float, MeteoPianoPitchState>> pitchStateSwitchPoint;
	MeteoPianoPitchState tempPitchState = MeteoPianoPitchState::None;
	//pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(-5.f, tempPitchState));	// ��Ĥ@�Ӯɶ��I�]���@�I�A�K�o�����q�O0��Ĥ@�ӭ�

	/* ��C��������s��control point */
	vector<ControlPoint*> controlPoints;
	map<Pitch, NoteControlPoint*> pressedNote;	// �O�����ӭ����U�F�٨S��}
	InputKeyControlPoint* pedalDown = nullptr;	// ������O�}�l��U���ɶ��C�p�G��O�S��U�A�N�]��nullptr
	

	/* ���и̭���l����T��X�� */
	for (int i = 0; i < workingSm.GetValue()->GetSm()->GetEvents()->size(); i++) {
		if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))) {
			
			/* �ƻs��O��T */
			if (useOriginalPedalData &&
				dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::SustainPedal) {
				
				controlPoints.push_back(dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

			}
			/* �ƻs�����K�׸�T */
			else if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::LowerOctave ||
					 dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::RaiseOctave) {

				controlPoints.push_back(dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

				float tempTime = workingSm.GetValue()->GetSm()->GetEvents()->at(i)->GetStartTime();

				/* �ΨӰO���ثe�b���@�ӤK�� */
				if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::LowerOctave) {
					if (tempPitchState == MeteoPianoPitchState::None) {
						tempPitchState = MeteoPianoPitchState::Lowered;
						pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(tempTime, tempPitchState));
					}
					else if (tempPitchState == MeteoPianoPitchState::Raised) {
						tempPitchState = MeteoPianoPitchState::None;
						pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(tempTime, tempPitchState));
					}
				}
				else if(dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::RaiseOctave) {
					if (tempPitchState == MeteoPianoPitchState::None) {
						tempPitchState = MeteoPianoPitchState::Raised;
						pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(tempTime, tempPitchState));
					}
					else if (tempPitchState == MeteoPianoPitchState::Lowered) {
						tempPitchState = MeteoPianoPitchState::None;
						pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(tempTime, tempPitchState));
					}
				}
				/* �ΨӰO���ثe�b���@�ӤK�� - end */
			}
		}
		/* �ƻs�p�`��T */
		else if (hasSectionData) {

			if (dynamic_cast<SectionStartControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))) {

				controlPoints.push_back(dynamic_cast<SectionStartControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

			}
		}
	}



	Ruleset* ruleset = rulesetInfo.GetValue()->CreateRuleset();
	MeteorPitchBindingSet* pitchBindingSet = dynamic_cast<MeteorPitchBindingSet*>(ruleset->GetDefaultPitchBindingSet());

	/* �ഫreplay frame���C������ */
	for (int i = 0; i < score->replay->replayFrames.size(); i++) {

		MeteorReplayFrame* replayFrame = dynamic_cast<MeteorReplayFrame*>(score->replay->replayFrames[i]);
		
		/* �p�G�o�ӭ������줣�P�A�N�����K�� */
		for (int j = 0; j < pitchStateSwitchPoint.size(); j++) {
			if (i < score->replay->replayFrames.size() - 1)
			if (score->replay->replayFrames[ i ]->GetStartTime() >= pitchStateSwitchPoint[j].first &&
				score->replay->replayFrames[i+1]->GetStartTime() <= pitchStateSwitchPoint[j].first) {
				pitchBindingSet->SwitchPitchState(pitchStateSwitchPoint[j].second);
			}
		}

		/* �O�^�䪺�� */
		if (replayFrame->GetMeteorAction() >= MeteorAction::VK27_A1 && 
			replayFrame->GetMeteorAction() < MeteorAction::SustainPedal) {
			
			Pitch pitch = pitchBindingSet->GetPitch(replayFrame->GetMeteorAction());

			map<Pitch, NoteControlPoint*>::iterator iter = pressedNote.find(pitch);
			/* �p�G����U�A�N���[�JpressedNote��map�� */
			if (iter == pressedNote.end() &&
				replayFrame->GetIsPressingDown()) {

				NoteControlPoint* noteControlPoint = new NoteControlPoint(pitch, replayFrame->GetStartTime(), 0.1f);
				noteControlPoint->SetVolume(replayFrame->GetVolume());
				pressedNote[pitch] = noteControlPoint;

			}
			/* �p�G�w�g���U�A���n��}�A�N���[�JcontrolPoints���� */
			else if (iter != pressedNote.end() &&
					 !replayFrame->GetIsPressingDown()) {

				pressedNote[pitch]->SetLifeTime(replayFrame->GetStartTime() - pressedNote[pitch]->GetStartTime());
				controlPoints.push_back(pressedNote[pitch]);
				pressedNote.erase(pitch);

			}
			/* �p�G�w�g���U�A�S����@�����U�A�N�����ª�note�[�JcontrolPoints�A�A��s��note��JpressedNote */
			else if (iter != pressedNote.end() &&
					 replayFrame->GetIsPressingDown()) {

				if (replayFrame->GetStartTime() - pressedNote[pitch]->GetStartTime() > 5)
					pressedNote[pitch]->SetLifeTime(5);
				else
					pressedNote[pitch]->SetLifeTime(replayFrame->GetStartTime() - pressedNote[pitch]->GetStartTime());
				controlPoints.push_back(pressedNote[pitch]);
				pressedNote.erase(pitch);

				NoteControlPoint* noteControlPoint = new NoteControlPoint(pitch, replayFrame->GetStartTime(), 0.1f);
				noteControlPoint->SetVolume(replayFrame->GetVolume());
				pressedNote[pitch] = noteControlPoint;
			}
		}
		/* �O��O���� */
		else if (replayFrame->GetMeteorAction() == MeteorAction::SustainPedal &&
				 !useOriginalPedalData) {

			/* �p�G���U�A�N���]�wpedalDownTime */
			if (pedalDown == nullptr &&
				replayFrame->GetIsPressingDown()) {

				pedalDown = new InputKeyControlPoint(InputKey::SustainPedal, replayFrame->GetStartTime(), 0.1f);

			}
			/* �p�G�w�g��U�A���n��}�A�N���[�JcontrolPoints���� */
			else if (pedalDown == nullptr &&
					 !replayFrame->GetIsPressingDown()) {

				pedalDown->SetLifeTime(replayFrame->GetStartTime() - pedalDown->GetStartTime());
				controlPoints.push_back(pedalDown);
				pedalDown = nullptr;

			}
			/* �p�G�w�g��U�A�S����@����U�A�N�����ª�pedal down�[�JcontrolPoints�A�A�طs��pedal down */
			else if (pedalDown != nullptr &&
					 replayFrame->GetIsPressingDown()) {

				pedalDown->SetLifeTime(replayFrame->GetStartTime() - pedalDown->GetStartTime() - 0.1f);
				controlPoints.push_back(pedalDown);
				
				pedalDown = new InputKeyControlPoint(InputKey::SustainPedal, replayFrame->GetStartTime(), 0.1f);

			}
		}
	}

	/* �ѤU�٨S�[�J�����[�@�[ */
	map<Pitch, NoteControlPoint*>::iterator iter;
	for (iter = pressedNote.begin(); iter != pressedNote.end(); ++iter) {

		(*iter).second->SetLifeTime(5);
		controlPoints.push_back((*iter).second);

	}
	pressedNote.clear();

	/* �ѤU�٨S�[�J����O�[�@�[ */
	if (pedalDown != nullptr)
		pedalDown->SetLifeTime(5);
	pedalDown = nullptr;

	/* ���s�Ƨ�(�p�`�}�l�ƥ�n�\�b���Ũƥ�e��) */
	sort(controlPoints.begin(), controlPoints.end(),
		[](ControlPoint* const& a, ControlPoint* const& b) {

		if (a->GetStartTime() == b->GetStartTime()) {
			if (a->Cast<SectionStartControlPoint>())
				return true;
			else if (b->Cast<SectionStartControlPoint>())
				return false;
		}

		return a->GetStartTime() < b->GetStartTime();
	});

	/* ��replay�ഫ��control point */
	convertToControlPoints(controlPoints);

	/* �ФW�p�`�O�� */
	tagSectionData(controlPoints);

	/* �s�X�g�J�ɮ� */
	string filePath = encodeToRecordFile(controlPoints);

	return filePath;
}

int MeteorResult::onEntering(Scene * lastScene)
{
	LOG(LogLevel::Info) << "MeteorResult::onEntering : pushing game over message.";

	/* �g�J�C������ */
	string recordFilePath = writeGameRecord();


	/* �٨S�g�n
	// bluetooth���e���G
	MeteoBluetoothMessage* meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinalScore);
	outputManager->PushMessage(meteoBluetoothMessage);

	meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PlayRecordData);
	outputManager->PushMessage(meteoBluetoothMessage);

	string recordFilePath;

	meteoBluetoothMessage = new MeteoFileBluetoothMessage(MeteoCommand::PlayRecordFileSegment, recordFilePath);
	outputManager->PushMessage(meteoBluetoothMessage);
	*/

	Exit();

	return 0;
}
