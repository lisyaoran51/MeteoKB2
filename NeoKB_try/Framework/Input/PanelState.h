#ifndef PANEL_STATE_H
#define PANEL_STATE_H

#include "PeripheralState.h"
#include <vector>


using namespace std;


namespace Framework {
namespace Input {

	enum class PanelButton;
	enum class PanelKnob;
	enum class PanelSlider;
	

	class PanelState : public PeripheralState<PanelState> {

		vector<PanelButton> buttons;

		vector<pair<PanelKnob,int>> knobs;

		vector<pair<PanelSlider, int>> sliders;


	public:

		int AddButton(PanelButton button);

		vector<PanelButton>* GetButtons();

		int AddKnob(pair<PanelKnob, int> knob);

		vector<pair<PanelKnob, int>>* GetKnobs();

		int AddSlider(pair<PanelSlider, int> slider);

		vector<pair<PanelSlider, int>>* GetSliders();

		virtual PanelState* Clone();


		bool ContainButton(PanelButton button);

		bool ContainSlider(PanelKnob knob);

		bool ContainerSlider(PanelSlider slider);



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