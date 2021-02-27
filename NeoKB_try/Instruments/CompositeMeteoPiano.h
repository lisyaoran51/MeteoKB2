#ifndef COMPOSITE_METEO_PIANO_H
#define COMPOSITE_METEO_PIANO_H


#include "MeteoPiano.h"
#include "VirtualMeteoPiano.h"
#include "Input/Handlers/InstrumentInputHandler.h"

using namespace Instruments::Input::Handlers;


namespace Instruments {

	/// <summary>
	/// ��composite pattern��Meteo piano�Mvirtual piano���X�A�@�_�A���`�OMeteo piano�A���ݭn�ɥi�H���Xvirtual piano�ϥ�
	/// �קK�h���Meteo piano�̭��쥻���]�p
	///	</summary>
	class CompositeMeteoPiano : public MeteoPiano {

		int load();

	public:

		CompositeMeteoPiano(vector<string>& args);


		/* ----------------------- Instrument.h ----------------------- */

		/// <summary>
		/// �o��n�إ�instant keyboard input handler�A�[��game host�̭��A���L���keyboard�W�A�o��keyboard�@����key down�N�i�H�ߨ��o����n�����X��
		///	</summary>
		virtual int SetHost(GameHost* h);

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);

		virtual int Sleep();

		virtual int WakeUp();



		/* ----------------------- Piano.h ----------------------- */

		virtual PassThroughInputManager* CreateInputManager();

		/// <summary>
		/// �⭵�ĦW�٩M�����j�b�@�_
		///	</summary>
		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0);

		/// <summary>
		/// ���J�׶s�M���^�O��j�b�@�_
		///	</summary>
		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		virtual int ChangeSustainType(SustainType sType);

		virtual SustainType GetSustainType();

		virtual int ControlSustainPedal(bool down);

		/// <summary>
		///	MeteoPianoPitchState�γo�ӧ�Apiano�����D�����X��State�A�u�౱��{�b�n���W���٬O���U��
		/// </summary>
		virtual int MoveOctave(PianoPitchMovement s);

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet);


		/* ----------------------- MeteoPiano.h ----------------------- */

		virtual int SetGameControllingPitchState(bool value);

		virtual int ChangePitchState(MeteoPianoPitchState s);


		/* ----------------------- CompositeMeteoPiano.h ----------------------- */

		VirtualMeteoPiano* GetVirtualMeteoPiano();

		MeteoPiano* GetMeteoPiano();

	protected:


		MeteoPiano* meteoPiano = nullptr;

		VirtualMeteoPiano* virtualMeteoPiano = nullptr;

		InstrumentInputHandler* instrumentInputHandler = nullptr;

		/// <summary>
		///	�Ӳz�ӻ��o�ӷ|�bload on complete�ɰ���A����piano�Mvirtual piano���|����@���A������o���٭n�S�O�b���L�̰���@���H
		/// </summary>
		virtual int loadAndMapSamples();

		/// <summary>
		///	�Ӳz�ӻ��o�ӷ|�bload on complete�ɰ���A����piano�Mvirtual piano���|����@���A������o���٭n�S�O�b���L�̰���@���H
		/// </summary>
		virtual int loadAndMapPitches();

	};

}








#endif