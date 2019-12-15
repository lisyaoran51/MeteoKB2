#ifndef METEO_PIANO_H
#define METEO_PIANO_H

#include "Piano.h"


using namespace Framework::Input::KeyBindings;


namespace Instruments {

	enum class MeteoPianoPitchState;

	class MeteoPiano : public Piano {

		bool isGameControllingPitchState = false;

		MeteoPianoPitchState state = MeteoPianoPitchState::None;

		map<PianoAction, SampleChannel*> raisedSamples;

		map<PianoAction, SampleChannel*> loweredSamples;

	public:

		MeteoPiano(vector<string>& args);

		int SetGameControllingPitchState(bool value);

		int ChangePitchState(MeteoPianoPitchState s);

		virtual int OnButtonDown(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);


	protected:

		virtual map<PianoAction, SampleChannel*>* getSamples();


		virtual int loadAndMapSamples();


	};

	enum class MeteoPianoPitchState {
		None,
		Raised,
		Lowered
	};

}




#endif