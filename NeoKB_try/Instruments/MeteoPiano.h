#ifndef METEO_PIANO_H
#define METEO_PIANO_H

#include "Piano.h"


using namespace Framework::Input::KeyBindings;


namespace Instruments {

	enum class MeteoPianoPitchState;

	class MeteoPiano : public Piano {

		bool isGameControllingPitchState = false;

		MeteoPianoPitchState state;

		map<PianoAction, SampleChannel*> raisedSamples;

		map<PianoAction, SampleChannel*> loweredSamples;

	public:

		int ChangePitchState(MeteoPianoPitchState s);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

		virtual int OnSlide(pair<PianoAction, int> action);


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