#ifndef VIRTUAL_METEO_PIANO_H
#define VIRTUAL_METEO_PIANO_H


#include "Piano.h"


namespace Instruments {

	enum class VirtualMeteoPianoSustainType {

		/// <summary>
		/// �L����
		///	</summary>
		None,
		/// <summary>
		/// ��������
		///	</summary>
		Auto,
		/// <summary>
		/// �C�������O
		///	</summary>
		Pedal	
	};

	class VirtualMeteoPiano : public Piano {

	public:

		VirtualMeteoPiano();

		/* ----------------------- Instrument.h ----------------------- */


		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);	// �o���٨S�g

		/* ----------------------- Piano.h ----------------------- */

		virtual int MoveOctave(PianoPitchMovement m);

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet);

		/* ----------------------- VirtualMeteoPiano.h ----------------------- */

		int SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType sType);

		VirtualMeteoPianoSustainType GetVirtualMeteoPianoSustainType();

		int Play(Pitch p, float volume);

		int Stop(Pitch action);

		int PressPedal();

		int ReleasePedal();

	protected:

		/* ----------------------- Instrument.h ----------------------- */

		virtual int loadAndMapPitches();	// �o���٨S�g

		/* ----------------------- Piano.h ----------------------- */

		virtual int resetState();

		/* ----------------------- VirtualMeteoPiano.h ----------------------- */

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