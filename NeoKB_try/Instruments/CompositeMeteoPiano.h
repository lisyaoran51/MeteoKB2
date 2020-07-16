#ifndef COMPOSITE_METEO_PIANO_H
#define COMPOSITE_METEO_PIANO_H


#include "MeteoPiano.h"
#include "VirtualMeteoPiano.h"


namespace Instruments {

	/// <summary>
	/// 用composite pattern把Meteo piano和virtual piano結合再一起，平常是Meteo piano，但需要時可以取出virtual piano使用
	/// 避免去更改Meteo piano裡面原本的設計
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
		/// 把音效名稱和音高綁在一起
		///	</summary>
		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0);

		/// <summary>
		/// 把輸入案鈕和鋼琴是件綁在一起
		///	</summary>
		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		int ChangeSustainType(SustainType sType);

		int ControlSustainPedal(bool down);

		int SetGameControllingPitchState(bool value);

		/// <summary>
		///	MeteoPianoPitchState用這個改，piano不知道有哪幾種State，只能控制現在要往上移還是往下移
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