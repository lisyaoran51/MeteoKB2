#ifndef PANEL_STATE_H
#define PANEL_STATE_H

#include "PeripheralState.h"
#include <vector>
#include "KeyBindings\InputKey.h"

using namespace std;
using namespace Framework::Input::KeyBindings;



namespace Framework {
namespace Input {

	

	class PanelState : public PeripheralState<PanelState> {

		vector<InputKey> buttons;

		vector<pair<InputKey,int>> knobs;

		vector<pair<InputKey, int>> sliders;


	public:

		int AddButton(InputKey button);

		vector<InputKey>* GetButtons();

		int AddKnob(pair<InputKey, int> knob);

		vector<pair<InputKey, int>>* GetKnobs();

		int AddSlider(pair<InputKey, int> slider);

		vector<pair<InputKey, int>>* GetSliders();

		virtual PanelState* Clone();


		bool ContainButton(InputKey button);

		bool ContainSlider(InputKey knob);

		bool ContainerSlider(InputKey slider);



	};

	enum class PanelButton {
		Power,
		Sensitive,
		Sustain,
		Lower,
		Higher

	};

	enum class PanelKnob {
		Measure,
		Speed
	};

	enum class PanelSlider {
		PianoVolume,
		TrackVolume
	};
}}





#endif