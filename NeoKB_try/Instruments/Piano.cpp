#include "Piano.h"

#include "Input\PianoAction.h"


using namespace Instruments;
using namespace Instruments::Input;



vector<SoundBinding*>* Piano::GetDefaultSoundBindings()
{
	vector<SoundBinding*>* bindings = new vector<SoundBinding*>();

	// TODO: 這邊要等file name寫好以後再寫
	bindings->push_back(new SoundBinding("filename", (int)PianoAction::None));

	/* key */
	for (int i = (int)InputKey::VK27_A1; i <= (int)InputKey::VK37_C4; i++) {
		bindings->push_back(new SoundBinding((InputKey)i, i));
	}

	/* pedal */
	for (int i = (int)InputKey::SustainPedal; i <= (int)InputKey::ExpressionPedal; i++) {
		bindings->push_back(new SoundBinding((InputKey)i, i));
	}

	/* Panel */
	for (int i = (int)InputKey::RaiseOctave; i <= (int)InputKey::Bluetooth; i++) {
		bindings->push_back(new SoundBinding((InputKey)i, i));
	}

	return bindings;
}

vector<KeyBinding*>* Instruments::Piano::GetDefaultkeyBindings(int variant)
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

	return bindings;
}
