#ifndef METEO_PIANO_H
#define METEO_PIANO_H

#include "Piano.h"
#include "MeteoPianoPitchState.h"


using namespace Framework::Input::KeyBindings;


namespace Instruments {

	class MeteoPiano : public Piano {

		bool isGameControllingPitchState = false;

		MeteoPianoPitchState state = MeteoPianoPitchState::None;

		map<PianoAction, SampleChannel*> raisedSamples;

		map<PianoAction, SampleChannel*> loweredSamples;

	public:

		MeteoPiano(vector<string>& args);

		/* ----------------------- Instrument.h ----------------------- */

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);	// 這個還沒寫

		/* ----------------------- MeteoPiano.h ----------------------- */

		virtual int SetGameControllingPitchState(bool value);

		/// <summary>
		///	MeteoPianoPitchState用這個改，piano不知道有哪幾種State，只能控制現在要往上移還是往下移
		/// </summary>
		virtual int MoveOctave(PianoPitchMovement s);

		virtual int ChangePitchState(MeteoPianoPitchState s);

		


	protected:

		virtual map<PianoAction, SampleChannel*>* getSamples(int variant = 0);


		virtual int loadAndMapSamples();

		virtual int loadAndMapPitches();

		virtual int OnButtonDown(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

	};



}




#endif