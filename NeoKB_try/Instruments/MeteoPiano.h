#ifndef METEO_PIANO_H
#define METEO_PIANO_H

#include "Piano.h"

using namespace Framework::Input::KeyBindings;


namespace Instruments {

	enum class PianoPitchState;

	class MeteoPiano : public Piano {

		bool isGameControllingPitchState = false;

		PianoPitchState state;

		map<PianoAction, SampleChannel*> raisedSamples;

		map<PianoAction, SampleChannel*> loweredSamples;

	public:

		int ChangePitchState(PianoPitchState s);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

		virtual int OnSlide(pair<PianoAction, int> action);


	protected:

		virtual map<PianoAction, SampleChannel*>* getSamples();


		virtual int loadAndMapSamples();


	};

	enum class PianoPitchState {
		None,
		Raised,
		Lowered
	};

}




#endif