#ifndef COMPOSITE_METEO_PIANO_H
#define COMPOSITE_METEO_PIANO_H


#include "MeteoPiano.h"
#include "VirtualMeteoPiano.h"


namespace Instruments {

	/// <summary>
	/// ��composite pattern��Meteo piano�Mvirtual piano���X�A�@�_�A���`�OMeteo piano�A���ݭn�ɥi�H���Xvirtual piano�ϥ�
	/// �קK�h���Meteo piano�̭��쥻���]�p
	///	</summary>
	class CompositeMeteoPiano : public MeteoPiano {

		vector<string>& arguments;

		int load();

	public:

		CompositeMeteoPiano(vector<string>& args);


		/* ----------------------- Instrument.h ----------------------- */

		int SetHost(GameHost* h);


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

		int ChangeSustainType(SustainType sType);

		int ControlSustainPedal(bool down);

		int SetGameControllingPitchState(bool value);

		/// <summary>
		///	MeteoPianoPitchState�γo�ӧ�Apiano�����D�����X��State�A�u�౱��{�b�n���W���٬O���U��
		/// </summary>
		int MoveOctave(PianoPitchMovement s);


		/* ----------------------- MeteoPiano.h ----------------------- */

		int ChangePitchState(MeteoPianoPitchState s);


		/* ----------------------- CompositeMeteoPiano.h ----------------------- */

		VirtualMeteoPiano* GetVirtualMeteoPiano();


	protected:


		MeteoPiano* meteoPiano = nullptr;

		VirtualMeteoPiano* virtualMeteoPiano = nullptr;

	};

}








#endif