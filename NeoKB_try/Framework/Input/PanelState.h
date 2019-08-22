#ifndef PANEL_STATE_H
#define PANEL_STATE_H

#include "PeripheralState.h"
#include <vector>


using namespace std;


namespace Framework {
namespace Input {

	

	class PanelState : public PeripheralState<PanelState> {

		vector<PanelButton> buttons;

		vector<pair<PanelKnob,int>> knobs;

		vector<pair<PanelSlider, int>> sliders;




	public:

		int AddButton(PanelButton button);

		vector<PanelButton>* GetButtons();

		int AddKnob(pair<PanelKnob, int> knob);

		vector<pair<PanelKnob, int>>* Getknobs();

		int AddSlider(pair<PanelSlider, int> slider);

		vector<pair<PanelSlider, int>>* GetSliders();

		virtual PanelState* Clone();




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