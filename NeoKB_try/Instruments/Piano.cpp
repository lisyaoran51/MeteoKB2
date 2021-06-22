#include "Piano.h"

#include "Input/PianoAction.h"
#include "Pitch.h"
#include "Input/PianoInputManager.h"
#include "../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "math.h"
#include "../Games/Output/Panels/IndicatorLightPanelMessage.h"


using namespace Instruments;
using namespace Instruments::Input;
using namespace Games::Output::Bluetooths;
using namespace Games::Output::Panels;



int Piano::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int Piano::load() : OutputManager not found in cache.");

	return load(o);
}

int Piano::load(OutputManager * o)
{
	outputManager = o;

	for (int i = (int)PianoAction::VK27_A1; i <= (int)PianoAction::VK37_C4; i++) {
		isPressingMap[(PianoAction)i] = false;
	}

	for (int i = (int)PianoAction::SustainPedal; i <= (int)PianoAction::ExpressionPedal; i++) {
		isPressingMap[(PianoAction)i] = false;
	}

	for (int i = (int)PianoAction::BluetoothPlugin; i <= (int)PianoAction::ExpressionPedalPlugin; i++) {
		isPressingMap[(PianoAction)i] = false;
	}

	onSleep.Add(this, [=]() {

		LOG(LogLevel::Debug) << "Piano::Lambda_OnSleep() : " << GetTypeName() << " reset state.";

		ChangeSustainType(SustainType::None);
		resetState();
		isSensitive = false;

		return 0;
	}, "Piano::Lambda_OnSleep");

	return 0;

	return 0;
}

Piano::Piano() : RegisterType("Piano")
{
	registerLoad(bind(static_cast<int(Piano::*)(void)>(&Piano::load), this));
}

PassThroughInputManager * Piano::CreateInputManager()
{
	return new PianoInputManager(this);
}

vector<SoundBinding*>* Piano::GetDefaultSoundBindings(int variant)
{
	vector<SoundBinding*>* bindings = new vector<SoundBinding*>();

	/* 這邊是先把假的音效裝進來，等到真的音效好了以後，再切換音效 */
	for (int i = (int)Pitch::A2; i < (int)Pitch::c5 + 1; i++) {
		bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch(i)));
	}

	return bindings;

	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::A2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::B2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::B2));

	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::C1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::D1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::D1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::E1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::E1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::F1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::G1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::G1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::A1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::A1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::B1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::B1));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::C));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::Db));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::D));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::Eb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::E));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::F));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::Gb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::G));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::Ab));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::A));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::Bb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::B));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::db));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::eb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::f));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::gb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::ab));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::bb));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::f1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a1));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b1b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b1));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::f2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a2));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b2b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b2));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d3b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e3b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::f3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g3b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a3b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a3));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b3b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b3));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d4b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::d4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e4b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::e4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::f4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g4b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::g4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a4b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::a4));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b4b));
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::b4));
													  
	bindings->push_back(new TSoundBinding<Pitch>("FakePianoSound", Pitch::c5));


	return bindings;
}

vector<KeyBinding*>* Piano::GetDefaultkeyBindings(int variant)
{
	vector<KeyBinding*>* bindings = new vector<KeyBinding*>();

	bindings->push_back(new KeyBinding(InputKey::None, (int)PianoAction::None));

	/* key */
	for (int i = (int)InputKey::VK27_A1; i <= (int)InputKey::VK37_C4; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* pedal */
	for (int i = (int)InputKey::SustainPedal; i <= (int)InputKey::ExpressionPedal; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* General Button */
	for (int i = (int)InputKey::Power; i <= (int)InputKey::Speed; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Panel */
	for (int i = (int)InputKey::RaiseOctave; i <= (int)InputKey::LowerOctave; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Plugin */
	for (int i = (int)InputKey::BluetoothPlugin; i <= (int)InputKey::ExpressionPedalPlugin; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	return bindings;
}

int Piano::SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet)
{

	LOG(LogLevel::Debug) << "Piano::SwitchSoundBindings() : switch piano sound to [" << sBindingSet->fileName << "].";

	if (sBindingSet == soundBindingSet) {

		// output manager -> 藍芽回傳這是一樣的音色
		return 0;
	}
	soundBindingSet = sBindingSet;


	/* 暫停任何輸入 */
	isActive == false;

	/* 重置鋼琴狀態 */
	resetState();

	/* 暫停sample manager更新 */
	audioManager->GetSampleManager()->SetIsActive(false);

	/* 更新sound binding音量 */
	audioManager->GetSampleManager()->GetVolume()->SetValue(soundBindingSet->Volume);

	/* 更新sound binding */
	for (int i = 0; i < soundBindings.size(); i++) {

		if(audioManager->GetSampleManager()->HasSampleChannel(soundBindings[i]))
			audioManager->GetSampleManager()->RemoveSampleChannel(soundBindings[i]);
		delete soundBindings[i];
	}
	soundBindings.clear();

	for (int i = (int)soundBindingSet->startKey; i <= (int)soundBindingSet->endKey; i++) {
		soundBindings.push_back(soundBindingSet->GetSoundBinding(Pitch(i)));
	}

	/* 重置smaple channel */
	//audioManager->GetSampleManager()->ClearSampleChannels();	// 不應該直接清空，有些sample可能不是樂器的，是其他音效

	loadAndMapSamples();

	/* 重啟sample manager更新 */
	audioManager->GetSampleManager()->SetIsActive(true);

	/* 重啟任何輸入 */
	isActive == true;

	return 0;
}

int Piano::ChangeSustainType(SustainType sType)
{
	sustainType = sType;

	if (sustainType == SustainType::AutoSustain) {

		IndicatorLightPanelMessage* indicatorLightMessage = nullptr;
		indicatorLightMessage = new IndicatorLightPanelMessage(2, true);
		outputManager->PushMessage(indicatorLightMessage);
	}
	else {

		IndicatorLightPanelMessage* indicatorLightMessage = nullptr;
		indicatorLightMessage = new IndicatorLightPanelMessage(2, false);
		outputManager->PushMessage(indicatorLightMessage);

	}

	// TODO: 用output manager把更改sustain type送到mcu

	return 0;
}

SustainType Piano::GetSustainType()
{
	return sustainType;
}

int Piano::ControlSustainPedal(bool down)
{
	LOG(LogLevel::Depricated) << "Piano::ControlSustainPedal() : make pedal [" << down << "].";

	if (sustainType == SustainType::GameControllingSustain) {
		isPressingMap[PianoAction::SustainPedal] = down;
		if (down){
			// 不用幹嘛
		}
		else {
			map<PianoAction, bool>::iterator it;
			for (it = isPressingMap.begin(); it != isPressingMap.end(); ++it) {
				LOG(LogLevel::Depricated) << "Piano::ControlSustainPedal() : pressing map has [" << (int)it->first << "]";
				if (!it->second) {
					
					if (getSamples()->find(it->first) != getSamples()->end()) {

						SampleChannel* sampleChannel = getSamples()->at(it->first);
						if (sampleChannel) {
							if (sampleChannel->GetIsPlaying()) {
								LOG(LogLevel::Debug) << "Piano::ControlSustainPedal() : pressing map has [" << (int)it->first << "] up by [" << GetTypeName() << "].";
								sampleChannel->FadeOut();
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

int Piano::SetSensitiveLevel(int sLevel)
{
	sensitiveLevel = sLevel;
	return 0;
}

int Piano::OnDirectKeyDown(pair<PianoAction, int> action)
{
	if (isSleeping)
		return -1;

	LOG(LogLevel::Depricated) << "Piano::OnKeyDown() : get fake input." << int(action.first);
	LOG(LogLevel::Depricated) << "Piano::OnKeyDown() : get key [" << int(action.first) << "] on velocity [" << action.second << "]";

	//getSamples()->at(action.first)->Play();
	if (getSamples()->find(action.first) != getSamples()->end()) {
		getSamples()->at(action.first)->Play(isSensitive ? pow(double(action.second) / 128.0, 1.0 / sensitiveLevel) : double(action.second) / 128.0);
		for (map<PianoAction, SampleChannel*>::iterator iter = getSamples()->begin(); iter != getSamples()->end(); ++iter) {
			LOG(LogLevel::Depricated) << "Piano::OnDirectKeyDown() : piano action [" << (int)iter->first << "] has sample [" << iter->second << "] by [" << GetTypeName() << "].";
		}
	}
	else
		return -1; // 無此按鍵

	isPressingMap[action.first] = true;
	return 0;
}

int Piano::resetState()
{
	map<PianoAction, bool>::iterator it;
	for (it = isPressingMap.begin(); it != isPressingMap.end(); ++it) {
		(*it).second = false;
	}

	map<PianoAction, SampleChannel*>::iterator it2;
	for (it2 = getSamples()->begin(); it2 != getSamples()->end(); ++it2) {

		(*it2).second->FadeOut();
	}

	return 0;
}

int Piano::update()
{
	
	return 0;
}

int Piano::OnKeyDown(pair<PianoAction, int> action)
{
	// 移到on direct key down去了
	return 0;
}

int Piano::OnKeyUp(PianoAction action)
{
	if (isSleeping)
		return -1;

	LOG(LogLevel::Finest) << "Piano::OnKeyUp() : handling fade out.";
	if (getSamples()->find(action) == getSamples()->end()) {
		LOG(LogLevel::Error) << "Piano::OnKeyUp() : not found action [" << (int)action << "] in samples by [" << GetTypeName() <<  "].";
		for (map<PianoAction, SampleChannel*>::iterator iter = getSamples()->begin(); iter != getSamples()->end(); ++iter) {
			LOG(LogLevel::Depricated) << "Piano::OnKeyUp() : piano action [" << (int)iter->first << "] has sample [" << iter->second << "] by [" << GetTypeName() << "].";
		}

		return -1;
	}

	// 沒踏踏板、有插踏板、沒開啟自動延音
	if (!(sustainType == SustainType::SustainPedal && isPressingMap.at(PianoAction::SustainPedal)) &&
		!(sustainType == SustainType::GameControllingSustain && isPressingMap.at(PianoAction::SustainPedal)) &&
		sustainType != SustainType::AutoSustain )
		getSamples()->at(action)->FadeOut();
		//getSamples()->at(action)->Stop();

	isPressingMap[action] = false;
	return 0;
}

int Piano::OnButtonDown(PianoAction action)
{
	if (isSleeping)
		return -1;

	/* 是否要設定按下，有些狀況不要設定按下 */
	bool isSetTempPressing = true;

	/* 沿音 */
	if (action == PianoAction::Sustain) {

		LOG(LogLevel::Depricated) << "Piano::OnButtonDown() : change sustain type. temp type is [" << (int)sustainType <<"]";

		if (sustainType == SustainType::GameControllingSustain) {

			// TODO: 回傳失敗，遊戲控制的情況下無法切換game control sustain
			LOG(LogLevel::Debug) << "Piano::OnButtonDown() : cannot switch to auto sustain when game controlling.";
		}
		else {
			MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::KeyboardIOEvent);

			json context;

			if (sustainType == SustainType::AutoSustain) {

				if (isPressingMap[PianoAction::SustainPedalPlugin]){
					sustainType = SustainType::SustainPedal;
				}
				else
					sustainType = SustainType::None;

				context.push_back(string("1001,0"));	// 0代表關閉
				meteoContextBluetoothMessage->SetContextInJson(context);

				IndicatorLightPanelMessage* indicatorLightMessage = new IndicatorLightPanelMessage(2, false);
				outputManager->PushMessage(indicatorLightMessage);
			}
			else {
				sustainType = SustainType::AutoSustain;

				context.push_back(string("1001,1"));	// 1代表開啟
				meteoContextBluetoothMessage->SetContextInJson(context);

				IndicatorLightPanelMessage* indicatorLightMessage = new IndicatorLightPanelMessage(2, true);
				outputManager->PushMessage(indicatorLightMessage);
			}
			meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
			outputManager->PushMessage(meteoContextBluetoothMessage);

		}

		LOG(LogLevel::Debug) << "Piano::OnButtonDown() : change sustain type to [" << (int)sustainType << "]";
	}
	/* 力度 */
	if (action == PianoAction::Sensitivity) {

		IndicatorLightPanelMessage* indicatorLightMessage = nullptr;
		//MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PianoPressSensitiveButton);
		if (isSensitive == true) {
			isSensitive = false;
			indicatorLightMessage = new IndicatorLightPanelMessage(1, false);
			outputManager->PushMessage(indicatorLightMessage);

			//json context;
			//context["State"] = false;
			//meteoContextBluetoothMessage->SetContextInJson(context);
		}
		else {
			isSensitive = true;
			indicatorLightMessage = new IndicatorLightPanelMessage(1, true);
			outputManager->PushMessage(indicatorLightMessage);
			//json context;
			//context["State"] = false;
			//meteoContextBluetoothMessage->SetContextInJson(context);
		}
		//meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);
		//outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	/* 插入踏板 */
	if (action == PianoAction::SustainPedalPlugin) {
		ChangeSustainType(SustainType::SustainPedal);
		LOG(LogLevel::Debug) << "Piano::OnButtonDown() : switch to sustain pedal.";

	}

	/* 踏下踏板 */
	if (action == PianoAction::SustainPedal) {

		LOG(LogLevel::Debug) << "Piano::OnButtonDown() : temp sustain type is [" << int(sustainType) << "].";

		// 如果目前是電腦控制踏板，就先停止接收踏板訓皓
		// 這邊程式有點問題，應該是不管怎樣都可以接收到踏下，只不過不會作用在聲音上
		if (sustainType == SustainType::AutoSustain || sustainType == SustainType::GameControllingSustain) {
			isSetTempPressing = false;
		}

	}


	if(isSetTempPressing)
		isPressingMap[action] = true;

	return 0;
}

int Piano::OnButtonUp(PianoAction action)
{
	if (isSleeping)
		return -1;
	
	bool isSetTempReleasing = true;

	// 如果目前是電腦控制踏板，就先停止接收踏板訓皓
	if (action == PianoAction::SustainPedal) {

		LOG(LogLevel::Debug) << "Piano::OnButtonUp() : temp sustain type is [" << int(sustainType) << "].";

		if (sustainType == SustainType::GameControllingSustain)
			isSetTempReleasing = false;

		if (sustainType == SustainType::SustainPedal) {

			LOG(LogLevel::Debug) << "Piano::OnButtonUp() : release pedal.";

			map<PianoAction, bool>::iterator it;
			for (it = isPressingMap.begin(); it != isPressingMap.end(); it++) {
				if (!it->second) {

					if (getSamples()->find(it->first) == getSamples()->end())
						continue;
					SampleChannel* sampleChannel = getSamples()->at(it->first);
					if (!sampleChannel)
						continue;
					if (!sampleChannel->GetIsPlaying())
						continue;

					LOG(LogLevel::Debug) << "Piano::OnButtonUp() : pressing map has [" << (int)it->first << "] up by [" << GetTypeName() << "].";
					sampleChannel->FadeOut();
					
				}
			}
		}
	}

	if (action == PianoAction::SustainPedalPlugin) {
		ChangeSustainType(SustainType::AutoSustain);
	}

	

	if(isSetTempReleasing)
		isPressingMap[action] = false;

	return 0;
}

int Piano::OnKnobTurn(pair<PianoAction, int> action)
{

	if (isSleeping)
		return -1;

	// 不需做任何動作
	return 0;
}

int Piano::OnSlide(pair<PianoAction, int> action)
{
	if (isSleeping)
		return -1;

	if (action.first == PianoAction::PianoVolumeSlider) {
		// 調整音量
	}

	return 0;
}

int Piano::onMessage(MeteoBluetoothMessage * message)
{
	return 0;
}
