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
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */


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


	Storage * s = GetCache<Storage>("Storage");
	if (!s)
		throw runtime_error("MeteorResult::load() : Storage not found in cache.");

	return load(o, i, s);
}

int MeteorResult::load(OutputManager * o, Instrument * i, Storage* s)
{
	outputManager = o;
	piano = dynamic_cast<Piano*>(i);

	if (piano == nullptr) {
		LOG(LogLevel::Error) << "MeteorResult::load : Instrument cannot cast to piano.";
	}

	storage = s;

	return 0;
}

int MeteorResult::convertToControlPoints(vector<ControlPoint*>& controlPoints)
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
				else if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::RaiseOctave) {
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
				if (score->replay->replayFrames[i]->GetStartTime() >= pitchStateSwitchPoint[j].first &&
					score->replay->replayFrames[i + 1]->GetStartTime() <= pitchStateSwitchPoint[j].first) {
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

	return 0;
}

int MeteorResult::tagSectionData(vector<ControlPoint*>& controlPoints)
{
	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;

	if (!hasSectionData)
		return 0;

	/* �O�_�ϥέ�l��O��T�A�p�G�и̥��ӴN����O��T�A�^�S�S����O�A�N�έ�l��O��T */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
								piano->GetSustainType() == SustainType::GameControllingSustain;




	int tempSectionIndex = 0;

	for (int i = 0; i < controlPoints.size(); i++) {
		/* �p�G�O�p�`�ƥ�A�N��stempSectionIndex */
		if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			tempSectionIndex = dynamic_cast<SectionStartControlPoint*>(controlPoints[i])->GetSectionIndex();
		}
		else if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i]) != nullptr) {
			/* �p�G�O��O�A�N��s��O�p�` */
			if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::SustainPedal &&
				!useOriginalPedalData) {
				dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->SetSectionIndex(tempSectionIndex);
			}
			/* �p�G�O�ɭ��K�סA�N��s�ɭ��K�פp�` */
			else if(dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::LowerOctave ||
					dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::RaiseOctave){
				dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->SetSectionIndex(tempSectionIndex);
			}
		}
		/* �p�G�O���šA�N��s���Ťp�` */
		else if (dynamic_cast<NoteControlPoint*>(controlPoints[i]) != nullptr) {
			dynamic_cast<NoteControlPoint*>(controlPoints[i])->SetSectionIndex(tempSectionIndex);
		}
	}

	

	return 0;
}

string MeteorResult::encodeToRecordFile(vector<ControlPoint*>& controlPoints)
{
	string fileName;
	char randomFileName[8] = { 0 };

	srand(time(NULL));

	/* random 0~9 a~z A~Z���r */
	for (int i = 0; i < 8; i++) {
		int charInInt = rand() % 62;
		
		if (charInInt < 10)
			randomFileName[i] = charInInt + 48;
		else if(charInInt >= 10 && charInInt < 36)
			randomFileName[i] = charInInt - 10 + 65;
		else if(charInInt >= 36)
			randomFileName[i] = charInInt - 36 + 97;

	}

	LOG(LogLevel::Debug) << "MeteorResult::encodeToRecordFile : create file stream.";
	
	fileName = string(randomFileName, 8) + string(".mr");
	fstream* stream = storage->GetStream(string("temp/mr/") + fileName, FileAccess::Write, FileMode::Create);

	LOG(LogLevel::Debug) << "MeteorResult::encodeToRecordFile : create file stream over.";


	/* �ɮ׮榡 */
	*stream << "simple file format v0\n";

#pragma region GeneralSection

	*stream << "[General]\n";

	/* �C������ */
	*stream << "Mode:3\n";

	/* �O�_�ϥέ�l��O��T�A�p�G�и̥��ӴN����O��T�A�^�S�S����O�A�N�έ�l��O��T */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
								piano->GetSustainType() == SustainType::GameControllingSustain;
	bool usePluginPedal = piano->GetSustainType() == SustainType::SustainPedal;
	
	/* ��O��T */
	if (useOriginalPedalData || usePluginPedal)
		*stream << "Pedal:1\n";

	/* �p�`��T */
	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;
	if (hasSectionData)
		*stream << "Section:1\n";

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

	/* �ϥΤ��T */
	*stream << "HandType:" << (int)hand << "\n";

	/* ���׸�T */
	*stream << "Difficulty:" << (int)difficulty << "\n";

#pragma endregion

#pragma region MetadataSection

	*stream << "[Metadata]\n";

	string title = workingSm.GetValue()->GetSm()->GetSmMetadata()->Title;

	/* ���W��T */
	*stream << "Title:" << title << "\n";

	/* ���a��T */
	//*stream << "User:" << user << "\n";



#pragma endregion

#pragma region DifficultySection

	*stream << "[Difficulty]\n";

	float speed = workingSm.GetValue()->GetSm()->GetSmInfo()->difficuty->Speed;

	/* �t�׸�T */
	*stream << "Speed:" << (int)speed << "\n";

#pragma endregion

#pragma region ControlPointSection

	*stream << "[NoteControlPoints]\n";

	/* ���Ÿ�T */
	for (int i = 0; i < controlPoints.size(); i++) {

		string controlPoint = "";

		if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i]) != nullptr) {

			InputKeyControlPoint* inputKeyControlPoint = dynamic_cast<InputKeyControlPoint*>(controlPoints[i]);

			/* ��O��T */
			if (inputKeyControlPoint->GetInputKey() == InputKey::SustainPedal) {
				controlPoint = string("-1,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",5,") +	// ��O���ϥΤ�O5
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
			/* ���K�׸�T */
			else if (inputKeyControlPoint->GetInputKey() == InputKey::LowerOctave) {
				controlPoint = string("-2,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",6,") +	// �����K�ת��ϥΤ�O6
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
			/* �ɤK�׸�T */
			else if(inputKeyControlPoint->GetInputKey() == InputKey::RaiseOctave) {
				controlPoint = string("-3,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",6,") +	// �����K�ת��ϥΤ�O6
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
		}
		else if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			/* �p�`��T */
			SectionStartControlPoint* sectionStartControlPoint = dynamic_cast<SectionStartControlPoint*>(controlPoints[i]);
			controlPoint = string("-4,") + to_string(sectionStartControlPoint->GetStartTime()) + string(",") +
										   to_string(sectionStartControlPoint->GetLifeTime()) + string("-1,") +
										   to_string(sectionStartControlPoint->GetSectionIndex()) + string(",0,") +
										   to_string(sectionStartControlPoint->GetPartIndex()) + string("\n");
		}
		else if (dynamic_cast<NoteControlPoint*>(controlPoints[i]) != nullptr) {
			/* ���Ÿ�T */
			NoteControlPoint* noteControlPoint = dynamic_cast<NoteControlPoint*>(controlPoints[i]);
			controlPoint = to_string((int)noteControlPoint->GetPitch()) + string(",") + 
						   to_string(noteControlPoint->GetStartTime()) + string(",") +
						   to_string(noteControlPoint->GetLifeTime()) + string(",") +
						   to_string(noteControlPoint->GetVolume()) + string(",") +
						   to_string(noteControlPoint->GetSectionIndex()) + string(",0,") +
						   to_string(noteControlPoint->GetPartIndex()) + string("\n");

		}

		*stream << controlPoint;
	}


#pragma endregion

	stream->close();

	return string("temp/mr/") + fileName;
}

MeteorResult::MeteorResult(Score * s) : Result(s), RegisterType("MeteorResult")
{

	registerLoad(bind((int(MeteorResult::*)())&MeteorResult::load, this));
}

string MeteorResult::writeGameRecord()
{
	LOG(LogLevel::Debug) << "MeteorResult::writeGameRecord : converting to game record.";

	/* ��C��������s��control point */
	vector<ControlPoint*> controlPoints;

	

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
	MeteoScene::onEntering(lastScene);

	LOG(LogLevel::Info) << "MeteorResult::onEntering : pushing game over message.";


	LOG(LogLevel::Debug) << "MeteorResult::onEntering : score:" << score;

	// bluetooth���e���G
	MeteoContextBluetoothMessage* scoreMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinalScore);
	json context;
	context["Hit Amount"] = score->hits;
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 1";
	context["Max Hit Amount"] = score->maxHits;
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 2";
	context["Score"] = score->score;
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 3";
	context["Max Score"] = score->maxScore;
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 4";
	context["Accuracy"] = int(score->accuracy * 10000);
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 5";
	context["Combo"] = score->combo;
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 6";



	// �٭n�g�J�U�Ӥ��ƪ�����
	json judgementMiss, judgementBad, judgementOk, judgementGood, judgementGreat, judgementPerfect;
	judgementMiss["Result"] = "Miss";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Miss];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 7";
	judgementBad["Result"] = "Bad";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Bad];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 8";
	judgementOk["Result"] = "Ok";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Ok];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 9";
	judgementGood["Result"] = "Good";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Good];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 10";
	judgementGreat["Result"] = "Great";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Great];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 11";
	judgementPerfect["Result"] = "Perfect";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Perfect];
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 12";

	context["Hits"].push_back(judgementMiss);
	context["Hits"].push_back(judgementBad);
	context["Hits"].push_back(judgementOk);
	context["Hits"].push_back(judgementGood);
	context["Hits"].push_back(judgementGreat);
	context["Hits"].push_back(judgementPerfect);
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 13";

	scoreMessage->SetContextInJson(context);
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 14";
	scoreMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : 15";


	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Set Message Over.";

	outputManager->PushMessage(scoreMessage);
	


	/* �٨S�g�n
	meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PlayRecordData);
	outputManager->PushMessage(meteoBluetoothMessage);

	string recordFilePath;
	*/

	/* �g�J�C������ */
	// �o�䤧��n�令�orequest
	string recordFilePath = writeGameRecord();
	//
	//MeteoFileBluetoothMessage* recordFileMessage = new MeteoFileBluetoothMessage(MeteoCommand::PlayRecordFileSegment, recordFilePath);
	//outputManager->PushMessage(recordFileMessage);
	

	LOG(LogLevel::Info) << "MeteorResult::onEntering : exit.";
	Exit();

	return 0;
}
