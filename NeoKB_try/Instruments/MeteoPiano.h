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
		///	MeteoPianoPitchState�γo�ӧ�Apiano�����D�����X��State�A�u�౱��{�b�n���W���٬O���U��
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