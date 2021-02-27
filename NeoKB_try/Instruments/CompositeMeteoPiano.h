#ifndef COMPOSITE_METEO_PIANO_H
#define COMPOSITE_METEO_PIANO_H


#include "MeteoPiano.h"
#include "VirtualMeteoPiano.h"
#include "Input/Handlers/InstrumentInputHandler.h"

using namespace Instruments::Input::Handlers;


namespace Instruments {

	/// <summary>
	/// 用composite pattern把Meteo piano和virtual piano結合再一起，平常是Meteo piano，但需要時可以取出virtual piano使用
	/// 避免去更改Meteo piano裡面原本的設計
	///	</summary>
	class CompositeMeteoPiano : public MeteoPiano {

		int load();

	public:

		CompositeMeteoPiano(vector<string>& args);


		/* ----------------------- Instrument.h ----------------------- */

		/// <summary>
		/// 這邊要建立instant keyboard input handler，加到game host裡面，讓他串到keyboard上，這樣keyboard一收到key down就可以立刻到這邊把聲音播出來
		///	</summary>
		virtual int SetHost(GameHost* h);

		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0);

		virtual int Sleep();

		virtual int WakeUp();



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

		virtual int ChangeSustainType(SustainType sType);

		virtual SustainType GetSustainType();

		virtual int ControlSustainPedal(bool down);

		/// <summary>
		///	MeteoPianoPitchState用這個改，piano不知道有哪幾種State，只能控制現在要往上移還是往下移
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
		///	照理來說這個會在load on complete時執行，應該piano和virtual piano都會執行一次，為什麼這邊還要特別在讓他們執行一次？
		/// </summary>
		virtual int loadAndMapSamples();

		/// <summary>
		///	照理來說這個會在load on complete時執行，應該piano和virtual piano都會執行一次，為什麼這邊還要特別在讓他們執行一次？
		/// </summary>
		virtual int loadAndMapPitches();

	};

}








#endif