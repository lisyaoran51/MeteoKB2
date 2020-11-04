#ifndef VIRTUAL_METEO_PIANO_H
#define VIRTUAL_METEO_PIANO_H


#include "Piano.h"


namespace Instruments {

	enum class VirtualMeteoPianoSustainType {
		None,
		/// <summary>
		/// 全部延音
		///	</summary>
		Auto,
		/// <summary>
		/// 遊戲控制踏板
		///	</summary>
		Pedal	
	};

	class VirtualMeteoPiano : public Piano {

	public:

		VirtualMeteoPiano();

		/* ----------------------- Piano.h ----------------------- */

		virtual int MoveOctave(PianoPitchMovement m);

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet);

		/* ----------------------- VirtualMeteoPiano.h ----------------------- */

		int SetSustainType(VirtualMeteoPianoSustainType sType);

		int Play(Pitch p, float volume);

		int Stop(Pitch action);

		int PressPedal();

		int ReleasePedal();

	protected:

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