#ifndef PANEL_STATE_H
#define PANEL_STATE_H

#include "PeripheralState.h"
#include <vector>
#include "KeyBindings/InputKey.h"

using namespace std;
using namespace Framework::Input::KeyBindings;



namespace Framework {
namespace Input {

	

	class PanelState : public PeripheralState<PanelState> {

		/// <summary>
		/// 按鈕按下或放開，true是按下，false是放開
		/// </summary>
		vector<pair<InputKey, bool>> buttons;

		vector<pair<InputKey,int>> knobs;

		vector<pair<InputKey, int>> sliders;


	public:

		int AddButton(pair<InputKey, bool> button);

		vector<pair<InputKey, bool>>* GetButtons();

		int AddKnob(pair<InputKey, int> knob);

		vector<pair<InputKey, int>>* GetKnobs();

		int AddSlider(pair<InputKey, int> slider);

		vector<pair<InputKey, int>>* GetSliders();

		virtual PanelState* Clone();


		bool ContainButton(InputKey button);

		bool ContainKnob(InputKey knob);

		bool ContainSlider(InputKey slider);

		virtual bool CheckIsEmpty();

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