#ifndef METEO_PIANO_H
#define METEO_PIANO_H

#include "Piano.h"


using namespace Framework::Input::KeyBindings;


namespace Instruments {

	enum class MeteoPianoPitchState {
		None,
		Raised,
		Lowered
	};

	class MeteoPiano : public Piano {

		bool isGameControllingPitchState = false;

		MeteoPianoPitchState state = MeteoPianoPitchState::None;

		map<PianoAction, SampleChannel*> raisedSamples;

		map<PianoAction, SampleChannel*> loweredSamples;

	public:

		MeteoPiano(vector<string>& args);

		int SetGameControllingPitchState(bool value);

		/// <summary>
		///	MeteoPianoPitchState用這個改，piano不知道有哪幾種State，只能控制現在要往上移還是往下移
		/// </summary>
		int MoveOctave(PianoPitchMovement s);

		int ChangePitchState(MeteoPianoPitchState s);

		


	protected:

		virtual map<PianoAction, SampleChannel*>* getSamples(int variant = 0);


		virtual int loadAndMapSamples();


		virtual int OnButtonDown(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

	};



}




#endif