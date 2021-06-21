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

	if(variant == 0){
		// TODO: 這邊要等file name寫好以後再寫
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_021_083", Pitch::A2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_022_083", Pitch::B2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_023_083", Pitch::B2));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_024_083", Pitch::C1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_025_083", Pitch::D1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_026_083", Pitch::D1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_027_083", Pitch::E1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_028_083", Pitch::E1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_029_083", Pitch::F1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_030_083", Pitch::G1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_031_083", Pitch::G1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_032_083", Pitch::A1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_033_083", Pitch::A1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_034_083", Pitch::B1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_035_083", Pitch::B1));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_036_083", Pitch::C));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_037_083", Pitch::Db));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_038_083", Pitch::D));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_039_083", Pitch::Eb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_040_083", Pitch::E));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_041_083", Pitch::F));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_042_083", Pitch::Gb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_043_083", Pitch::G));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_044_083", Pitch::Ab));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_045_083", Pitch::A));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_046_083", Pitch::Bb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_047_083", Pitch::B));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_048_083", Pitch::c));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_049_083", Pitch::db));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_050_083", Pitch::d));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_051_083", Pitch::eb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_052_083", Pitch::e));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_053_083", Pitch::f));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_054_083", Pitch::gb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_055_083", Pitch::g));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_056_083", Pitch::ab));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_057_083", Pitch::a));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_058_083", Pitch::bb));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_059_083", Pitch::b));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_060_083", Pitch::c1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_061_083", Pitch::d1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_062_083", Pitch::d1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_063_083", Pitch::e1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_064_083", Pitch::e1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_065_083", Pitch::f1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_066_083", Pitch::g1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_067_083", Pitch::g1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_068_083", Pitch::a1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_069_083", Pitch::a1));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_070_083", Pitch::b1b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_071_083", Pitch::b1));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_072_083", Pitch::c2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_073_083", Pitch::d2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_074_083", Pitch::d2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_075_083", Pitch::e2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_076_083", Pitch::e2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_077_083", Pitch::f2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_078_083", Pitch::g2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_079_083", Pitch::g2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_080_083", Pitch::a2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_081_083", Pitch::a2));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_082_083", Pitch::b2b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_083_083", Pitch::b2));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_084_083", Pitch::c3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_085_083", Pitch::d3b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_086_083", Pitch::d3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_087_083", Pitch::e3b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_088_083", Pitch::e3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_089_083", Pitch::f3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_090_083", Pitch::g3b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_091_083", Pitch::g3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_092_083", Pitch::a3b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_093_083", Pitch::a3));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_094_083", Pitch::b3b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_095_083", Pitch::b3));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_096_083", Pitch::c4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_097_083", Pitch::d4b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_098_083", Pitch::d4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_099_083", Pitch::e4b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_100_083", Pitch::e4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_101_083", Pitch::f4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_102_083", Pitch::g4b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_103_083", Pitch::g4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_104_083", Pitch::a4b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_105_083", Pitch::a4));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_106_083", Pitch::b4b));
		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_107_083", Pitch::b4));

		bindings->push_back(new TSoundBinding<Pitch>("German_Concert_D_108_083", Pitch::c5));
	}																   
	else if (variant == 1) {
		bindings->push_back(new TSoundBinding<Pitch>("piano_9", Pitch::A2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_10", Pitch::B2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_11", Pitch::B2));

		bindings->push_back(new TSoundBinding<Pitch>("piano_12", Pitch::C1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_13", Pitch::D1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_14", Pitch::D1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_15", Pitch::E1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_16", Pitch::E1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_17", Pitch::F1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_18", Pitch::G1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_19", Pitch::G1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_20", Pitch::A1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_21", Pitch::A1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_22", Pitch::B1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_23", Pitch::B1));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_24", Pitch::C));
		bindings->push_back(new TSoundBinding<Pitch>("piano_25", Pitch::Db));
		bindings->push_back(new TSoundBinding<Pitch>("piano_26", Pitch::D));
		bindings->push_back(new TSoundBinding<Pitch>("piano_27", Pitch::Eb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_28", Pitch::E));
		bindings->push_back(new TSoundBinding<Pitch>("piano_29", Pitch::F));
		bindings->push_back(new TSoundBinding<Pitch>("piano_30", Pitch::Gb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_31", Pitch::G));
		bindings->push_back(new TSoundBinding<Pitch>("piano_32", Pitch::Ab));
		bindings->push_back(new TSoundBinding<Pitch>("piano_33", Pitch::A));
		bindings->push_back(new TSoundBinding<Pitch>("piano_34", Pitch::Bb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_35", Pitch::B));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_36", Pitch::c));
		bindings->push_back(new TSoundBinding<Pitch>("piano_37", Pitch::db));
		bindings->push_back(new TSoundBinding<Pitch>("piano_38", Pitch::d));
		bindings->push_back(new TSoundBinding<Pitch>("piano_39", Pitch::eb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_40", Pitch::e));
		bindings->push_back(new TSoundBinding<Pitch>("piano_41", Pitch::f));
		bindings->push_back(new TSoundBinding<Pitch>("piano_42", Pitch::gb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_43", Pitch::g));
		bindings->push_back(new TSoundBinding<Pitch>("piano_44", Pitch::ab));
		bindings->push_back(new TSoundBinding<Pitch>("piano_45", Pitch::a));
		bindings->push_back(new TSoundBinding<Pitch>("piano_46", Pitch::bb));
		bindings->push_back(new TSoundBinding<Pitch>("piano_47", Pitch::b));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_48", Pitch::c1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_49", Pitch::d1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_50", Pitch::d1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_51", Pitch::e1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_52", Pitch::e1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_53", Pitch::f1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_54", Pitch::g1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_55", Pitch::g1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_56", Pitch::a1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_57", Pitch::a1));
		bindings->push_back(new TSoundBinding<Pitch>("piano_58", Pitch::b1b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_59", Pitch::b1));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_60", Pitch::c2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_61", Pitch::d2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_62", Pitch::d2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_63", Pitch::e2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_64", Pitch::e2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_65", Pitch::f2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_66", Pitch::g2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_67", Pitch::g2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_68", Pitch::a2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_69", Pitch::a2));
		bindings->push_back(new TSoundBinding<Pitch>("piano_70", Pitch::b2b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_71", Pitch::b2));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_72", Pitch::c3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_73", Pitch::d3b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_74", Pitch::d3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_75", Pitch::e3b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_76", Pitch::e3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_77", Pitch::f3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_78", Pitch::g3b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_79", Pitch::g3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_80", Pitch::a3b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_81", Pitch::a3));
		bindings->push_back(new TSoundBinding<Pitch>("piano_82", Pitch::b3b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_83", Pitch::b3));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_84", Pitch::c4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_85", Pitch::d4b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_86", Pitch::d4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_87", Pitch::e4b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_88", Pitch::e4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_89", Pitch::f4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_90", Pitch::g4b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_91", Pitch::g4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_92", Pitch::a4b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_93", Pitch::a4));
		bindings->push_back(new TSoundBinding<Pitch>("piano_94", Pitch::b4b));
		bindings->push_back(new TSoundBinding<Pitch>("piano_95", Pitch::b4));
													  
		bindings->push_back(new TSoundBinding<Pitch>("piano_96", Pitch::c5));
	}


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
	if (!(sustainType == SustainType::SustainPedal && isPressingMap.at(PianoAction::SustainPedal)) 
		&& sustainType != SustainType::AutoSustain )
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
