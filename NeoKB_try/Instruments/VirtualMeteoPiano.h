#ifndef VIRTUAL_METEO_PIANO_H
#define VIRTUAL_METEO_PIANO_H


#include "Piano.h"


namespace Instruments {

	enum class VirtualMeteoPianoSustainType {
		None,
		Auto,
		Pedal
	};

	class VirtualMeteoPiano : public Piano {

	public:

		VirtualMeteoPiano();

		int SetSustainType(VirtualMeteoPianoSustainType sType);

		int Play(Pitch p, float volume);

		int Stop(Pitch action);

		int PressPedal();

		int ReleasePedal();

		virtual int MoveOctave(PianoPitchMovement m);

	protected:

		map<Pitch, SampleChannel*> samplesByPitch;

		map<Pitch, bool> isPressingMapByPitch;

		bool pedalDown = false;

		VirtualMeteoPianoSustainType sustainType = VirtualMeteoPianoSustainType::Auto;

		virtual map<PianoAction, SampleChannel*>* getSamples(int variant = 0);

		virtual map<Pitch, SampleChannel*>* getSamplesByPitch(int variant = 0);

		virtual int loadAndMapSamples();

		virtual int OnKeyDown(pair<PianoAction, int> action);

		virtual int OnKeyUp(PianoAction action);

		virtual int OnButtonDown(PianoAction action);

		virtual int OnButtonUp(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

		virtual int OnSlide(pair<PianoAction, int> action);

	};


}




#endif