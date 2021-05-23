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
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include "../../../Games/IO/Communications/PostBinaryBleRequest.h"
#include "../../../Util/StringSplitter.h"
#include <stdio.h>


using namespace Meteor::Scenes::Results;
using namespace Games::Output::Bluetooths;
using namespace Meteor::Rulesets::Modifiers;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Meteor::Input;
using namespace Meteor::Rulesets::Replays;
using namespace Games::IO::Communications;
using namespace Util;

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


	CommunicationAccess * c = GetCache<CommunicationAccess>("CommunicationAccess");
	if (!c)
		throw runtime_error("MeteorResult::load() : CommunicationAccess not found in cache.");

	return load(o, i, s, c);
}

int MeteorResult::load(OutputManager * o, Instrument * i, Storage* s, CommunicationAccess* c)
{
	outputManager = o;
	piano = dynamic_cast<Piano*>(i);

	if (piano == nullptr) {
		LOG(LogLevel::Error) << "MeteorResult::load : Instrument cannot cast to piano.";
	}

	storage = s;

	communicationAccess = c;

	return 0;
}

string MeteorResult::TrimOff(string s, int precision)
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

int MeteorResult::convertToControlPoints(vector<ControlPoint*>& controlPoints)
{
	/* 是否使用原始踏板資訊，如果譜裡本來就有踏板資訊，琴又沒插踏板，就用原始踏板資訊 */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
		piano->GetSustainType() == SustainType::GameControllingSustain;

	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;

	/* 這次遊戲是用哪隻手 */
	SmDifficultyHandType hand = SmDifficultyHandType::None;

	/* 這次遊戲難度為何 */
	SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

	for (int i = 0; i < workingSm.GetValue()->GetModifiers()->GetValue()->size(); i++) {
		if (dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			hand = dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetHandType();
		}
		if (dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			difficulty = dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetDifficulty();
		}
	}

	/* 用來記錄目前在哪一個八度 */
	vector<pair<float, MeteoPianoPitchState>> pitchStateSwitchPoint;
	MeteoPianoPitchState tempPitchState = MeteoPianoPitchState::None;
	//pitchStateSwitchPoint.push_back(pair<float, MeteoPianoPitchState>(-5.f, tempPitchState));	// 把第一個時間點設早一點，免得有的歌是0秒第一個音


	map<Pitch, NoteControlPoint*> pressedNote;	// 記錄哪個音按下了還沒放開
	InputKeyControlPoint* pedalDown = nullptr;	// 紀錄踏板開始踏下的時間。如果踏板沒踏下，就設為nullptr


	/* 把譜裡面原始的資訊抓出來 */
	for (int i = 0; i < workingSm.GetValue()->GetSm()->GetEvents()->size(); i++) {
		if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))) {

			/* 複製踏板資訊 */
			if (useOriginalPedalData &&
				dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::SustainPedal) {

				controlPoints.push_back(dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

			}
			/* 複製平移八度資訊 */
			else if (dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::LowerOctave ||
				dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))->GetInputKey() == InputKey::RaiseOctave) {

				controlPoints.push_back(dynamic_cast<InputKeyControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

				float tempTime = workingSm.GetValue()->GetSm()->GetEvents()->at(i)->GetStartTime();

				/* 用來記錄目前在哪一個八度 */
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
				/* 用來記錄目前在哪一個八度 - end */
			}
		}
		/* 複製小節資訊 */
		else if (hasSectionData) {

			if (dynamic_cast<SectionStartControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i))) {

				controlPoints.push_back(dynamic_cast<SectionStartControlPoint*>(workingSm.GetValue()->GetSm()->GetEvents()->at(i)));

			}
		}
	}

	LOG(LogLevel::Depricated) << [=]() {
		LOG(LogLevel::Debug) << "MeteorResult::convertToControlPoints : pitch state switch point";
		for (int i = 0; i < pitchStateSwitchPoint.size(); i++) {
			LOG(LogLevel::Debug) << "MeteorResult::convertToControlPoints : [" << pitchStateSwitchPoint[i].first << "] at state [" << int(pitchStateSwitchPoint[i].second) << "]";
		}
		return 0;
	}();


	Ruleset* ruleset = rulesetInfo.GetValue()->CreateRuleset();
	MeteorPitchBindingSet* pitchBindingSet = dynamic_cast<MeteorPitchBindingSet*>(ruleset->GetDefaultPitchBindingSet());

	/* 轉換replay frame成遊戲紀錄 */
	for (int i = 0; i < score->replay->replayFrames.size(); i++) {

		MeteorReplayFrame* replayFrame = dynamic_cast<MeteorReplayFrame*>(score->replay->replayFrames[i]);

		/* 如果這個音的音域不同，就切換八度 */
		for (int j = 0; j < pitchStateSwitchPoint.size(); j++) {
			if (i < score->replay->replayFrames.size() - 1)
				if (score->replay->replayFrames[i]->GetStartTime() <= pitchStateSwitchPoint[j].first &&
					score->replay->replayFrames[i + 1]->GetStartTime() >= pitchStateSwitchPoint[j].first) {
					pitchBindingSet->SwitchPitchState(pitchStateSwitchPoint[j].second);

					LOG(LogLevel::Depricated) << "MeteorResult::convertToControlPoints : pitch state switch to [" << (int)pitchStateSwitchPoint[j].second << "].";
				}
		}

		/* 是琴鍵的話 */
		if (replayFrame->GetMeteorAction() >= MeteorAction::VK27_A1 &&
			replayFrame->GetMeteorAction() < MeteorAction::SustainPedal) {

			Pitch pitch = pitchBindingSet->GetPitch(replayFrame->GetMeteorAction());

			map<Pitch, NoteControlPoint*>::iterator iter = pressedNote.find(pitch);
			/* 如果剛按下，就先加入pressedNote的map中 */
			if (iter == pressedNote.end() &&
				replayFrame->GetIsPressingDown()) {

				NoteControlPoint* noteControlPoint = new NoteControlPoint(pitch, replayFrame->GetStartTime(), 0.1f);
				noteControlPoint->SetVolume(replayFrame->GetVolume());
				pressedNote[pitch] = noteControlPoint;

				LOG(LogLevel::Depricated) << "MeteorResult::convertToControlPoints : get new pitch input [" << int(pitch) << "].";

			}
			/* 如果已經按下，正要放開，就先加入controlPoints中中 */
			else if (iter != pressedNote.end() &&
				!replayFrame->GetIsPressingDown()) {

				pressedNote[pitch]->SetLifeTime(replayFrame->GetStartTime() - pressedNote[pitch]->GetStartTime());
				controlPoints.push_back(pressedNote[pitch]);
				pressedNote.erase(pitch);

			}
			/* 如果已經按下，又收到一次按下，就先把舊的note加入controlPoints，再把新的note放入pressedNote */
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
		/* 是踏板的話 */
		else if (replayFrame->GetMeteorAction() == MeteorAction::SustainPedal &&
			!useOriginalPedalData) {

			/* 如果剛踏下，就先設定pedalDownTime */
			if (pedalDown == nullptr &&
				replayFrame->GetIsPressingDown()) {

				pedalDown = new InputKeyControlPoint(InputKey::SustainPedal, replayFrame->GetStartTime(), 0.1f);

			}
			/* 如果已經踏下，正要放開，就先加入controlPoints中中 */
			else if (pedalDown == nullptr &&
				!replayFrame->GetIsPressingDown()) {

				pedalDown->SetLifeTime(replayFrame->GetStartTime() - pedalDown->GetStartTime());
				controlPoints.push_back(pedalDown);
				pedalDown = nullptr;

			}
			/* 如果已經踏下，又收到一次踏下，就先把舊的pedal down加入controlPoints，再建新的pedal down */
			else if (pedalDown != nullptr &&
				replayFrame->GetIsPressingDown()) {

				pedalDown->SetLifeTime(replayFrame->GetStartTime() - pedalDown->GetStartTime() - 0.1f);
				controlPoints.push_back(pedalDown);

				pedalDown = new InputKeyControlPoint(InputKey::SustainPedal, replayFrame->GetStartTime(), 0.1f);

			}
		}
	}

	/* 剩下還沒加入的音加一加 */
	map<Pitch, NoteControlPoint*>::iterator iter;
	for (iter = pressedNote.begin(); iter != pressedNote.end(); ++iter) {

		(*iter).second->SetLifeTime(5);
		controlPoints.push_back((*iter).second);

	}
	pressedNote.clear();

	/* 剩下還沒加入的踏板加一加 */
	if (pedalDown != nullptr)
		pedalDown->SetLifeTime(5);
	pedalDown = nullptr;

	/* 重新排序(小節開始事件要擺在音符事件前面) */
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


	LOG(LogLevel::Debug) << "MeteorResult::convertToControlPoints : [" << controlPoints.size() << "] control points converted." << &controlPoints;

	return 0;
}

int MeteorResult::tagSectionData(vector<ControlPoint*>& controlPoints)
{
	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;

	if (!hasSectionData)
		return 0;

	/* 是否使用原始踏板資訊，如果譜裡本來就有踏板資訊，琴又沒插踏板，就用原始踏板資訊 */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
								piano->GetSustainType() == SustainType::GameControllingSustain;




	int tempSectionIndex = 0;

	for (int i = 0; i < controlPoints.size(); i++) {
		/* 如果是小節事件，就更新tempSectionIndex */
		if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			tempSectionIndex = dynamic_cast<SectionStartControlPoint*>(controlPoints[i])->GetSectionIndex();
		}
		else if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i]) != nullptr) {
			/* 如果是踏板，就更新踏板小節 */
			if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::SustainPedal &&
				!useOriginalPedalData) {
				dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->SetSectionIndex(tempSectionIndex);
			}
			/* 如果是升降八度，就更新升降八度小節 */
			else if(dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::LowerOctave ||
					dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->GetInputKey() == InputKey::RaiseOctave){
				dynamic_cast<InputKeyControlPoint*>(controlPoints[i])->SetSectionIndex(tempSectionIndex);
			}
		}
		/* 如果是音符，就更新音符小節 */
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

	/* random 0~9 a~z A~Z等字 */
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
	fstream* stream = storage->GetStream(string("MeteoRecords/") + fileName, FileAccess::Write, FileMode::Create);

	LOG(LogLevel::Debug) << "MeteorResult::encodeToRecordFile : create file stream over.";


	/* 檔案格式 */
	*stream << "simple file format v0\n";

#pragma region GeneralSection

	*stream << "[General]\n";

	/* 遊戲紀錄 */
	*stream << "Mode:4\n";

	/* 是否使用原始踏板資訊，如果譜裡本來就有踏板資訊，琴又沒插踏板，就用原始踏板資訊 */
	bool useOriginalPedalData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasPedalData &&
								piano->GetSustainType() == SustainType::GameControllingSustain;
	bool usePluginPedal = piano->GetSustainType() == SustainType::SustainPedal;
	
	/* 踏板資訊 */
	if (useOriginalPedalData || usePluginPedal)
		*stream << "Pedal:1\n";

	/* 小節資訊 */
	bool hasSectionData = workingSm.GetValue()->GetSm()->GetSmInfo()->hasSectionData;
	if (hasSectionData)
		*stream << "Section:1\n";

	/* 這次遊戲是用哪隻手 */
	SmDifficultyHandType hand = SmDifficultyHandType::None;

	/* 這次遊戲難度為何 */
	SmDifficultyDifficulty difficulty = SmDifficultyDifficulty::None;

	for (int i = 0; i < workingSm.GetValue()->GetModifiers()->GetValue()->size(); i++) {
		if (dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			hand = dynamic_cast<HandModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetHandType();
		}
		if (dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))) {
			difficulty = dynamic_cast<MeteorDifficultyModifier*>(workingSm.GetValue()->GetModifiers()->GetValue()->at(i))->GetDifficulty();
		}
	}

	/* 使用手資訊 */
	*stream << "HandType:" << (int)hand << "\n";

	/* 難度資訊 */
	*stream << "Difficulty:" << (int)difficulty << "\n";

#pragma endregion

#pragma region MetadataSection

	*stream << "[Metadata]\n";

	string title = workingSm.GetValue()->GetSm()->GetSmMetadata()->Title;

	/* 曲名資訊 */
	*stream << "Title:" << title << "\n";

	/* 玩家資訊 */
	//*stream << "User:" << user << "\n";



#pragma endregion

#pragma region DifficultySection

	*stream << "[Difficulty]\n";

	float speed = workingSm.GetValue()->GetSm()->GetSmInfo()->difficuty->Speed;

	/* 速度資訊 */
	*stream << "Speed:" << (int)speed << "\n";

#pragma endregion

#pragma region ControlPointSection

	*stream << "[NoteControlPoints]\n";

	/* 音符資訊 */
	for (int i = 0; i < controlPoints.size(); i++) {

		string controlPoint = "";

		if (dynamic_cast<InputKeyControlPoint*>(controlPoints[i]) != nullptr) {

			InputKeyControlPoint* inputKeyControlPoint = dynamic_cast<InputKeyControlPoint*>(controlPoints[i]);

			/* 踏板資訊 */
			if (inputKeyControlPoint->GetInputKey() == InputKey::SustainPedal) {
				controlPoint = string("-1,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
											   TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") + 
											   TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",5,") +	// 踏板的使用手是5
											   TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
			/* 降八度資訊 */
			else if (inputKeyControlPoint->GetInputKey() == InputKey::LowerOctave) {
				controlPoint = string("-2,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
											   TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") + 
											   TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",6,") +	// 平移八度的使用手是6
											   TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
			/* 升八度資訊 */
			else if(inputKeyControlPoint->GetInputKey() == InputKey::RaiseOctave) {
				controlPoint = string("-3,") + TrimOff(to_string(inputKeyControlPoint->GetStartTime()), 2) + string(",") +
											   TrimOff(to_string(inputKeyControlPoint->GetLifeTime()), 2) + string("-1,") + 
											   TrimOff(to_string(inputKeyControlPoint->GetSectionIndex()), 2) + string(",6,") +	// 平移八度的使用手是6
											   TrimOff(to_string(inputKeyControlPoint->GetPartIndex()), 2) + string("\n");
			}
		}
		else if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			/* 小節資訊 */
			SectionStartControlPoint* sectionStartControlPoint = dynamic_cast<SectionStartControlPoint*>(controlPoints[i]);
			controlPoint = string("-4,") + TrimOff(to_string(sectionStartControlPoint->GetStartTime()), 2) + string(",") +
										   TrimOff(to_string(sectionStartControlPoint->GetLifeTime()), 2) + string("-1,") +
										   TrimOff(to_string(sectionStartControlPoint->GetSectionIndex()), 2) + string(",0,") +
										   TrimOff(to_string(sectionStartControlPoint->GetPartIndex()), 2) + string("\n");
		}
		else if (dynamic_cast<NoteControlPoint*>(controlPoints[i]) != nullptr) {
			/* 音符資訊 */
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

MeteorResult::MeteorResult(Score * s) : Result(s), RegisterType("MeteorResult")
{

	registerLoad(bind((int(MeteorResult::*)())&MeteorResult::load, this));
}

string MeteorResult::writeGameRecord()
{
	LOG(LogLevel::Debug) << "MeteorResult::writeGameRecord : converting to game record.";

	/* 把遊戲紀錄轉存成control point */
	vector<ControlPoint*> controlPoints;

	

	/* 把replay轉換成control point */
	convertToControlPoints(controlPoints);


	LOG(LogLevel::Debug) << "MeteorResult::writeGameRecord : [" << controlPoints.size() << "] control points converted." << &controlPoints;

	/* 標上小節記號 */
	tagSectionData(controlPoints);

	/* 編碼寫入檔案 */
	string filePath = encodeToRecordFile(controlPoints);

	return filePath;
}

int MeteorResult::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	LOG(LogLevel::Info) << "MeteorResult::onEntering : pushing game over message.";

	// bluetooth推送結果
	MeteoContextBluetoothMessage* scoreMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinalScore);
	json context;
	context["Hit Amount"] = score->hits;
	context["Max Hit Amount"] = score->maxHits;
	context["Score"] = score->score;
	context["Max Score"] = score->maxScore;
	context["Accuracy"] = int(score->accuracy * 10000);
	context["Combo"] = score->combo;



	// 還要寫入各個分數的次數
	json judgementMiss, judgementBad, judgementOk, judgementGood, judgementGreat, judgementPerfect;
	judgementMiss["Result"] = "Miss";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Miss];
	judgementBad["Result"] = "Bad";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Bad];
	judgementOk["Result"] = "Ok";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Ok];
	judgementGood["Result"] = "Good";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Good];
	judgementGreat["Result"] = "Great";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Great];
	judgementPerfect["Result"] = "Perfect";	judgementMiss["HitAmount"] = score->hitResults[HitResult::Perfect];

	context["Hits"].push_back(judgementMiss);
	context["Hits"].push_back(judgementBad);
	context["Hits"].push_back(judgementOk);
	context["Hits"].push_back(judgementGood);
	context["Hits"].push_back(judgementGreat);
	context["Hits"].push_back(judgementPerfect);

	scoreMessage->SetContextInJson(context);
	scoreMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);


	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Set Message Over.";

	outputManager->PushMessage(scoreMessage);

	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Hit Amounts: [" << score->hits << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Max Hit Amounts: [" << score->maxHits << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Score: [" << score->score << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Max Score: [" << score->maxScore << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Accuracy: [" << int(score->accuracy * 10000) << "(" << score->accuracy << ")].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Combo: [" << score->combo << "].";

	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Miss: [" << score->hitResults[HitResult::Miss] << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Bad: [" << score->hitResults[HitResult::Bad] << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Ok: [" << score->hitResults[HitResult::Ok] << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Good: [" << score->hitResults[HitResult::Good] << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Great: [" << score->hitResults[HitResult::Great] << "].";
	LOG(LogLevel::Debug) << "MeteorResult::onEntering : Perfect: [" << score->hitResults[HitResult::Perfect] << "].";



	/* 還沒寫好
	meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PlayRecordData);
	outputManager->PushMessage(meteoBluetoothMessage);

	string recordFilePath;
	*/

	/* 寫入遊戲紀錄 */
	// 這邊之後要改成發request
	string recordFilePath = writeGameRecord();
	//
	//MeteoFileBluetoothMessage* recordFileMessage = new MeteoFileBluetoothMessage(MeteoCommand::PlayRecordFileSegment, recordFilePath);
	//outputManager->PushMessage(recordFileMessage);

	/*

	MeteoContextBluetoothMessage* postRecordMessage = new MeteoContextBluetoothMessage(MeteoCommand::PostPlayRecord);
	json requestContext;

	requestContext["FileName"] = StringSplitter::Split(recordFilePath, "/").back();
	postRecordMessage->SetContextInJson(requestContext);
	postRecordMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	PostBinaryBleRequest* postRecordRequest = new PostBinaryBleRequest(
		recordFilePath,
		postRecordMessage,
		MeteoCommand::AckPostPlayRecord,
		MeteoCommand::PlayRecordFileSegment,
		MeteoCommand::FinishWritePlayRecord,
		MeteoCommand::RequestRewritePlayRecordFileSegment,
		MeteoCommand::AckFinishWritePlayRecord
	);

	postRecordRequest->SetCallbackScene(this);

	postRecordRequest->AddOnSuccess(this, [=]() {

		FILE* fp = popen((string("rm -f ") + recordFilePath).c_str(), "r");
		if (fp == NULL) {
			// throw error
		}
		pclose(fp);

		LOG(LogLevel::Info) << "MeteorResult::onEntering : exit.";
		Exit();
		return 0;
	}, "PostRlayRecordSuccess");

	communicationAccess->Queue(postRecordRequest);

	*/

	Exit();

	return 0;
}
