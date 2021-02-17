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



	Ruleset* ruleset = rulesetInfo.GetValue()->CreateRuleset();
	MeteorPitchBindingSet* pitchBindingSet = dynamic_cast<MeteorPitchBindingSet*>(ruleset->GetDefaultPitchBindingSet());

	/* 轉換replay frame成遊戲紀錄 */
	for (int i = 0; i < score->replay->replayFrames.size(); i++) {

		MeteorReplayFrame* replayFrame = dynamic_cast<MeteorReplayFrame*>(score->replay->replayFrames[i]);

		/* 如果這個音的音域不同，就切換八度 */
		for (int j = 0; j < pitchStateSwitchPoint.size(); j++) {
			if (i < score->replay->replayFrames.size() - 1)
				if (score->replay->replayFrames[i]->GetStartTime() >= pitchStateSwitchPoint[j].first &&
					score->replay->replayFrames[i + 1]->GetStartTime() <= pitchStateSwitchPoint[j].first) {
					pitchBindingSet->SwitchPitchState(pitchStateSwitchPoint[j].second);
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
	fstream* stream = storage->GetStream(string("temp/mr/") + fileName, FileAccess::Write, FileMode::Create);

	LOG(LogLevel::Debug) << "MeteorResult::encodeToRecordFile : create file stream over.";


	/* 檔案格式 */
	*stream << "simple file format v0\n";

#pragma region GeneralSection

	*stream << "[General]\n";

	/* 遊戲紀錄 */
	*stream << "Mode:3\n";

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
				controlPoint = string("-1,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",5,") +	// 踏板的使用手是5
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
			/* 降八度資訊 */
			else if (inputKeyControlPoint->GetInputKey() == InputKey::LowerOctave) {
				controlPoint = string("-2,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",6,") +	// 平移八度的使用手是6
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
			/* 升八度資訊 */
			else if(inputKeyControlPoint->GetInputKey() == InputKey::RaiseOctave) {
				controlPoint = string("-3,") + to_string(inputKeyControlPoint->GetStartTime()) + string(",") +
											   to_string(inputKeyControlPoint->GetLifeTime()) + string("-1,") + 
											   to_string(inputKeyControlPoint->GetSectionIndex()) + string(",6,") +	// 平移八度的使用手是6
											   to_string(inputKeyControlPoint->GetPartIndex()) + string("\n");
			}
		}
		else if (dynamic_cast<SectionStartControlPoint*>(controlPoints[i]) != nullptr) {
			/* 小節資訊 */
			SectionStartControlPoint* sectionStartControlPoint = dynamic_cast<SectionStartControlPoint*>(controlPoints[i]);
			controlPoint = string("-4,") + to_string(sectionStartControlPoint->GetStartTime()) + string(",") +
										   to_string(sectionStartControlPoint->GetLifeTime()) + string("-1,") +
										   to_string(sectionStartControlPoint->GetSectionIndex()) + string(",0,") +
										   to_string(sectionStartControlPoint->GetPartIndex()) + string("\n");
		}
		else if (dynamic_cast<NoteControlPoint*>(controlPoints[i]) != nullptr) {
			/* 音符資訊 */
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

	/* 把遊戲紀錄轉存成control point */
	vector<ControlPoint*> controlPoints;

	

	/* 把replay轉換成control point */
	convertToControlPoints(controlPoints);

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


	LOG(LogLevel::Debug) << "MeteorResult::onEntering : score:" << score;

	// bluetooth推送結果
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



	// 還要寫入各個分數的次數
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
	

	LOG(LogLevel::Info) << "MeteorResult::onEntering : exit.";
	Exit();

	return 0;
}
