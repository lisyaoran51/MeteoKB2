#include "Piano.h"

#include "Input\PianoAction.h"
#include "Pitch.h"


using namespace Instruments;
using namespace Instruments::Input;



int Piano::load()
{
	for (int i = (int)PianoAction::VK27_A1; i <= (int)PianoAction::VK37_C4; i++) {
		isPressingMap[(PianoAction)i] = false;
	}

	for (int i = (int)PianoAction::SustainPedal; i <= (int)PianoAction::ExpressionPedal; i++) {
		isPressingMap[(PianoAction)i] = false;
	}

	for (int i = (int)PianoAction::BluetoothPlugin; i <= (int)PianoAction::ExpressionPedalPlugin; i++) {
		isPressingMap[(PianoAction)i] = false;
	}
	return 0;
}

Piano::Piano() : RegisterType("Piano")
{
	registerLoad(bind(static_cast<int(Piano::*)(void)>(&Piano::load), this));
}

vector<SoundBinding*>* Piano::GetDefaultSoundBindings(int variant)
{
	vector<SoundBinding*>* bindings = new vector<SoundBinding*>();

	if(variant == 0){
		// TODO: 硂娩璶单file name糶糶
		bindings->push_back(new TSoundBinding<Pitch>("Piano_9" , Pitch::A2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_10", Pitch::B2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_11", Pitch::B2 ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_12", Pitch::C1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_13", Pitch::D1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_14", Pitch::D1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_15", Pitch::E1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_16", Pitch::E1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_17", Pitch::F1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_18", Pitch::G1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_19", Pitch::G1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_20", Pitch::A1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_21", Pitch::A1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_22", Pitch::B1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_23", Pitch::B1 ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_24", Pitch::C  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_25", Pitch::Db ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_26", Pitch::D  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_27", Pitch::Eb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_28", Pitch::E  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_29", Pitch::F  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_30", Pitch::Gb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_31", Pitch::G  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_32", Pitch::Ab ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_33", Pitch::A  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_34", Pitch::Bb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_35", Pitch::B  ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_36", Pitch::c  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_37", Pitch::db ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_38", Pitch::d  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_39", Pitch::eb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_40", Pitch::e  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_41", Pitch::f  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_42", Pitch::gb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_43", Pitch::g  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_44", Pitch::ab ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_45", Pitch::a  ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_46", Pitch::bb ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_47", Pitch::b  ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_48", Pitch::c1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_49", Pitch::d1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_50", Pitch::d1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_51", Pitch::e1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_52", Pitch::e1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_53", Pitch::f1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_54", Pitch::g1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_55", Pitch::g1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_56", Pitch::a1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_57", Pitch::a1 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_58", Pitch::b1b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_59", Pitch::b1 ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_60", Pitch::c2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_61", Pitch::d2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_62", Pitch::d2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_63", Pitch::e2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_64", Pitch::e2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_65", Pitch::f2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_66", Pitch::g2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_67", Pitch::g2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_68", Pitch::a2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_69", Pitch::a2 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_70", Pitch::b2b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_71", Pitch::b2 ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_72", Pitch::c3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_73", Pitch::d3b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_74", Pitch::d3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_75", Pitch::e3b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_76", Pitch::e3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_77", Pitch::f3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_78", Pitch::g3b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_79", Pitch::g3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_80", Pitch::a3b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_81", Pitch::a3 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_82", Pitch::b3b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_83", Pitch::b3 ));

		bindings->push_back(new TSoundBinding<Pitch>("Piano_84", Pitch::c4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_85", Pitch::d4b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_86", Pitch::d4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_87", Pitch::e4b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_88", Pitch::e4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_89", Pitch::f4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_90", Pitch::g4b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_91", Pitch::g4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_92", Pitch::a4b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_93", Pitch::a4 ));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_94", Pitch::b4b));
		bindings->push_back(new TSoundBinding<Pitch>("Piano_95", Pitch::b4 ));
																
		bindings->push_back(new TSoundBinding<Pitch>("Piano_96", Pitch::c5));
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

	/* Panel */
	for (int i = (int)InputKey::RaiseOctave; i <= (int)InputKey::Bluetooth; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	/* Plugin */
	for (int i = (int)InputKey::BluetoothPlugin; i <= (int)InputKey::ExpressionPedalPlugin; i++) {
		bindings->push_back(new KeyBinding((InputKey)i, i));
	}

	return bindings;
}

int Piano::ChangeSustainType(SustainType sType)
{
	sustainType = sType;

	return 0;
}

int Piano::ControlSustainPedal(bool down)
{
	if (sustainType == SustainType::GameControllingSustain) {
		isPressingMap[PianoAction::SustainPedal] = down;
		if (down){
			// ぃノ稦估
		}
		else {
			map<PianoAction, bool>::iterator it;
			for (it = isPressingMap.begin(); it != isPressingMap.end(); it++) {
				if (!it->second) {
					SampleChannel* sampleChannel = getSamples()->at(it->first);
					if (sampleChannel)
						sampleChannel->Stop();
				}

			}
		}
		
	}
	return 0;
}



int Piano::OnKeyDown(pair<PianoAction, int> action)
{
	getSamples()->at(action.first)->Play();
	isPressingMap[action.first] = true;
	return 0;
}

int Piano::OnKeyUp(PianoAction action)
{
	// ⊿今今狾Τ础今狾⊿秨币笆┑
	if(!isPressingMap.at(PianoAction::SustainPedal) && !isAutoSustain )
		getSamples()->at(action)->Stop();

	isPressingMap[action] = false;
	return 0;
}

int Piano::OnButtonDown(PianoAction action)
{
	// 狦ヘ玡琌筿福北今狾碞氨ゎ钡Μ今狾癡祋
	if (sustainType == SustainType::GameControllingSustain && action == PianoAction::SustainPedal)
		return 0;

	isPressingMap[action] = true;
	if (action == PianoAction::SustainPedalPlugin) {
		// mainInterface->GetPanel()->ChangeState(PianoAction::SustainButton, false);
	}

	return 0;
}

int Piano::OnButtonUp(PianoAction action)
{
	// 狦ヘ玡琌筿福北今狾碞氨ゎ钡Μ今狾癡祋
	if (sustainType == SustainType::GameControllingSustain && action == PianoAction::SustainPedal)
		return 0;

	isPressingMap[action] = false;
	if (action == PianoAction::SustainPedal && sustainType == SustainType::SustainPedal) {
		map<PianoAction, bool>::iterator it;
		for (it = isPressingMap.begin(); it != isPressingMap.end(); it++) {
			if (!it->second) {
				SampleChannel* sampleChannel = getSamples()->at(it->first);
				if (sampleChannel)
					sampleChannel->Stop();
			}

		}
	}

	return 0;
}
