#include "RecordResult.h"

#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Output/Bluetooths/MeteoFileBluetoothMessage.h"
#include "../../../Games/Scheduler/Event/ControlPoints/SectionStartControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Instruments/MeteoPianoPitchState.h"
#include "../../Input/RecordInputManager.h"
#include "../../Input/RecordPitchBindingSet.h"
#include "../../Ruleset/Replays/RecordReplayFrame.h"
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */
#include "../../../Games/IO/Communications/PostBinaryBleRequest.h"
#include "../../../Util/StringSplitter.h"
#include <stdio.h>
#include "../../../Games/IO/Communications/GetBinaryBleRequestHandler.h"


using namespace Record::Scenes::Results;
using namespace Games::Output::Bluetooths;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Instruments;
using namespace Record::Input;
using namespace Record::Rulesets::Replays;
using namespace Games::IO::Communications;
using namespace Util;

int RecordResult::load()
{
	Instrument * i = GetCache<Instrument>("Instrument");
	if (!i)
		throw runtime_error("MeteorResult::load() : Instrument not found in cache.");


	Storage * s = GetCache<Storage>("Storage");
	if (!s)
		throw runtime_error("MeteorResult::load() : Storage not found in cache.");


	CommunicationAccess * c = GetCache<CommunicationAccess>("CommunicationAccess");
	if (!c)
		throw runtime_error("MeteorResult::load() : CommunicationAccess not found in cache.");

	return load(i, s, c);
}

int RecordResult::load(Instrument * i, Storage* s, CommunicationAccess* c)
{

	piano = dynamic_cast<Piano*>(i);

	if (piano == nullptr) {
		LOG(LogLevel::Error) << "MeteorResult::load : Instrument cannot cast to piano.";
	}

	storage = s;

	communicationAccess = c;

	return 0;
}

string RecordResult::TrimOff(string s, int precision)
{
	if (s.find(".") == string::npos)
		return s;

	for (int i = s.length() - 1; i > s.find("."); i--) {
		if (s[i] == '0')
			s = s.substr(0, i);
	}

	if (s[s.length() - 1] == '.') {
		return s.substr(0, s.length() - 1);
	}

	if (s.length() > s.find(".") + precision - 1) {
		return s.substr(0, s.find(".") + precision + 1);
	}

	return s;
}

int RecordResult::convertToControlPoints(vector<ControlPoint*>& controlPoints)
{
	/* �ΨӰO���ثe�b���@�ӤK�� */
	vector<pair<float, MeteoPianoPitchState>> pitchStateSwitchPoint;
	MeteoPianoPitchState tempPitchState = MeteoPianoPitchState::None;
	//pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(-5.f, tempPitchState));	// ��Ĥ@�Ӯɶ��I�]���@�I�A�K�o�����q�O0��Ĥ@�ӭ�


	map<Pitch, NoteControlPoint*> pressedNote;	// �O�����ӭ����U�F�٨S��}
	InputKeyControlPoint* pedalDown = nullptr;	// ������O�}�l��U���ɶ��C�p�G��O�S��U�A�N�]��nullptr



	Ruleset* ruleset = rulesetInfo.GetValue()->CreateRuleset();
	RecordPitchBindingSet* pitchBindingSet = dynamic_cast<RecordPitchBindingSet*>(ruleset->GetDefaultPitchBindingSet());

	/* �ഫreplay frame���C������ */
	for (int i = 0; i < score->replay->replayFrames.size(); i++) {

		RecordReplayFrame* replayFrame = dynamic_cast<RecordReplayFrame*>(score->replay->replayFrames[i]);

		/* �p�G�o�ӭ������줣�P�A�N�����K�� */
		for (int j = 0; j < pitchStateSwitchPoint.size(); j++) {
			if (i < score->replay->replayFrames.size() - 1)
				if (score->replay->replayFrames[i]->GetStartTime() <= pitchStateSwitchPoint[j].first &&
					score->replay->replayFrames[i + 1]->GetStartTime() >= pitchStateSwitchPoint[j].first) {
					pitchBindingSet->SwitchPitchState(pitchStateSwitchPoint[j].second);

					LOG(LogLevel::Depricated) << "MeteorResult::convertToControlPoints : pitch state switch to [" << (int)pitchStateSwitchPoint[j].second << "].";
				}
		}

		/* �O�^�䪺�� */
		if (replayFrame->GetRecordAction() >= RecordAction::VK27_A1 &&
			replayFrame->GetRecordAction() < RecordAction::SustainPedal) {

			Pitch pitch = pitchBindingSet->GetPitch(replayFrame->GetRecordAction());

			map<Pitch, NoteControlPoint*>::iterator iter = pressedNote.find(pitch);
			/* �p�G����U�A�N���[�JpressedNote��map�� */
			if (iter == pressedNote.end() &&
				replayFrame->GetIsPressingDown()) {

				NoteControlPoint* noteControlPoint = new NoteControlPoint(pitch, replayFrame->GetStartTime(), 0.1f);
				noteControlPoint->SetVolume(replayFrame->GetVolume());
				pressedNote[pitch] = noteControlPoint;

				LOG(LogLevel::Depricated) << "MeteorResult::convertToControlPoints : get new pitch input [" << int(pitch) << "].";

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

				LOG(LogLevel::Depricated) << "MeteorResult::convertToControlPoints : get new pitch input [" << int(pitch) << "].";
			}
		}
		/* �O��O���� */
		else if (replayFrame->GetRecordAction() == RecordAction::SustainPedal) {

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


	LOG(LogLevel::Debug) << "RecordResult::convertToControlPoints : [" << controlPoints.size() << "] control points converted." << &controlPoints;

	return 0;
}

int RecordResult::tagSectionData(vector<ControlPoint*>& controlPoints)
{
	
	return 0;
}

string RecordResult::encodeToRecordFile(vector<ControlPoint*>& controlPoints)
{
	string fileName;
	char randomFileName[8] = { 0 };

	srand(time(NULL));

	/* random 0~9 a~z A~Z���r */
	for (int i = 0; i < 8; i++) {
		int charInInt = rand() % 62;

		if (charInInt < 10)
			randomFileName[i] = charInInt + 48;
		else if (charInInt >= 10 && charInInt < 36)
			randomFileName[i] = charInInt - 10 + 65;
		else if (charInInt >= 36)
			randomFileName[i] = charInInt - 36 + 97;

	}

	LOG(LogLevel::Debug) << "RecordResult::encodeToRecordFile : create file stream.";

	fileName = string(randomFileName, 8) + string(".mr");
	fstream* stream = storage->GetStream(string("MeteoRecords/") + fileName, FileAccess::Write, FileMode::Create);

	LOG(LogLevel::Debug) << "RecordResult::encodeToRecordFile : create file stream over.";


	/* �ɮ׮榡 */
	*stream << "simple file format v0\n";

#pragma region GeneralSection

	*stream << "[General]\n";

	/* �C������ */
	*stream << "Mode:4\n";

	/* �O�_�ϥέ�l��O��T�A�p�G�и̥��ӴN����O��T�A�^�S�S����O�A�N�έ�l��O��T */
	bool usePluginPedal = piano->GetSustainType() == SustainType::SustainPedal;

	/* ��O��T */
	if (usePluginPedal)
		*stream << "Pedal:1\n";

	/* �p�`��T */

	/* �o���C���O�έ����� */

	/* �o���C�����׬��� */

	/* �ϥΤ��T */

	/* ���׸�T */

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

	/* �t�׸�T */

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
				controlPoint = string("-1,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
					TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") +
					TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",5,") +	// ��O���ϥΤ�O5
					TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
			/* ���K�׸�T */
			else if (inputKeyControlPoint->GetInputKey() == InputKey::LowerOctave) {
				controlPoint = string("-2,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
					TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") +
					TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",6,") +	// �����K�ת��ϥΤ�O6
					TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
			/* �ɤK�׸�T */
			else if (inputKeyControlPoint->GetInputKey() == InputKey::RaiseOctave) {
				controlPoint = string("-3,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
					TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") +
					TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",6,") +	// �����K�ת��ϥΤ�O6
					TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
		}
		else if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			/* �p�`��T */
			SectionStartControlPoint* sectionStartControlPoint = dynamic_cast<SectionStartControlPoint*>(controlPoints[i]);
			controlPoint = string("-4,") + TrimOff(to_string(sectionStartControlPoint->GetStartTime()), 2) + string(",") +
				TrimOff(to_string(sectionStartControlPoint->GetLifeTime()), 2) + string("-1,") +
				TrimOff(to_string(sectionStartControlPoint->GetSectionIndex()), 2) + string(",0,") +
				TrimOff(to_string(sectionStartControlPoint->GetPartIndex()), 2) + string("\n");
		}
		else if (dynamic_cast<NoteControlPoint*>(controlPoints[i]) != nullptr) {
			/* ���Ÿ�T */
			NoteControlPoint* noteControlPoint = dynamic_cast<NoteControlPoint*>(controlPoints[i]);
			controlPoint = TrimOff(to_string((int)noteControlPoint->GetPitch()), 2) + string(",") +
				TrimOff(to_string(noteControlPoint->GetStartTime()), 2) + string(",") +
				TrimOff(to_string(noteControlPoint->GetLifeTime()), 2) + string(",") +
				TrimOff(to_string(noteControlPoint->GetVolume()), 2) + string(",") +
				TrimOff(to_string(noteControlPoint->GetSectionIndex()), 2) + string(",0,") +
				TrimOff(to_string(noteControlPoint->GetPartIndex()), 2) + string("\n");

		}

		*stream << controlPoint;
	}


#pragma endregion

	stream->close();
	delete stream;

	return string("MeteoRecords/") + fileName;
}

RecordResult::RecordResult(Score * s) : Result(s), RegisterType("MeteorResult")
{

	registerLoad(bind((int(RecordResult::*)())&RecordResult::load, this));
}

string RecordResult::writeGameRecord()
{
	LOG(LogLevel::Debug) << "RecordResult::writeGameRecord : converting to game record.";

	/* ��C��������s��control point */
	vector<ControlPoint*> controlPoints;



	/* ��replay�ഫ��control point */
	convertToControlPoints(controlPoints);


	LOG(LogLevel::Debug) << "RecordResult::writeGameRecord : [" << controlPoints.size() << "] control points converted." << &controlPoints;

	/* �ФW�p�`�O�� */
	tagSectionData(controlPoints);

	/* �s�X�g�J�ɮ� */
	string filePath = encodeToRecordFile(controlPoints);

	return filePath;
}

int RecordResult::onEntered(Scene * lastScene)
{
	LOG(LogLevel::Info) << "RecordResult::onEntering : pushing game over message.";
	MeteoScene::onEntered(lastScene);


	// ����A�ӭ�
#if 0

	/* �g�J�C������ */
	// �o�䤧��n�令�orequest
	string recordFilePath = writeGameRecord();
	vector<string> path = StringSplitter::Split(recordFilePath, "/");
	string fileName = path.back();
	path.pop_back();
	string directoryPath = StringSplitter::Combine(path, "/");

	MeteoContextBluetoothMessage* playRecordDataMessage = new MeteoContextBluetoothMessage(MeteoCommand::PlayRecordData);
	json context;

	context["FileName"] = fileName;
	context["SheetmusicName"] = workingSm.GetValue()->GetSm()->GetSmMetadata()->Title;

	playRecordDataMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
	outputManager->PushMessage(playRecordDataMessage);


	GetBinaryBleRequestHandler* getPlayRecordBleRequestHandler = new GetBinaryBleRequestHandler(
		directoryPath,
		fileName,
		MeteoCommand::RequestPlayRecordFile,
		MeteoCommand::AckRequestPlayRecordFile,
		MeteoCommand::PlayRecordFileSegment,
		MeteoCommand::AckPlayRecordFileSegment,
		MeteoCommand::FinishWritePlayRecord,
		MeteoCommand::RequestRewritePlayRecordFileSegment,
		MeteoCommand::AckFinishWritePlayRecord
	);

	getPlayRecordBleRequestHandler->SetCallbackScene(this);

	getPlayRecordBleRequestHandler->AddOnSuccess(this, [=](string s) {

		FILE* fp = popen((string("rm -f ") + directoryPath + "/" + s).c_str(), "r");
		if (fp == NULL) {
			// throw error
		}
		pclose(fp);

		LOG(LogLevel::Info) << "RecordResult::onEntering : post record success. exit.";
		Exit();
		return 0;
	}, "Lambda_RecordResult::GetPlayRecordBleRequestHandleSuccess");

	getPlayRecordBleRequestHandler->AddOnFail(this, [=](string s) {

		FILE* fp = popen((string("rm -f ") + directoryPath + "/" + s).c_str(), "r");
		if (fp == NULL) {
			// throw error
		}
		pclose(fp);

		LOG(LogLevel::Info) << "RecordResult::onEntering : post record failed. exit.";
		Exit();
		return 0;
	}, "Lambda_RecordResult::GetPlayRecordBleRequestHandleFail");

	communicationAccess->Queue(getPlayRecordBleRequestHandler);

#endif

	//Exit();

	return 0;
}

int RecordResult::onMessage(MeteoBluetoothMessage * message)
{
	return Result::onMessage(message);
}
