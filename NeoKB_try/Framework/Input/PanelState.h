#ifndef PANEL_STATE_H
#define PANEL_STATE_H

#include "PeripheralState.h"

namespace Framework {
namespace Input {


	class PanelState : public PeripheralState<PanelState> {

		vector<PanelButton> buttons;

		vector<pair<PanelKnob,int>> knobs;

		vector<pair<PanelSlider, int>> sliders;




	public:

		vector<PanelButton>* GetButtons();

		vector<pair<PanelKnob, int>>* Getknobs();

		vector<pair<PanelSlider, int>>* GetSliders();

		virtual PanelState* Clone();




	};

	enum class Button {


	};

	enum class Knob {


	};

	enum class Slider {


	};
}}





#endif