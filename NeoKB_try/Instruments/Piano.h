#ifndef PIANO_H
#define PIANO_H

#include "Instrument.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"
#include "Input/PianoAction.h"
#include "../Framework/Output/OutputManager.h"

using namespace Framework::Input::KeyBindings;
using namespace Instruments::Input;
using namespace Framework::Output;


namespace Instruments {


	enum class SustainType {

		/// <summary>
		/// 沒有sustain
		///	</summary>
		None,

		/// <summary>
		/// 用踏板
		///	</summary>
		SustainPedal,

		/// <summary>
		/// 自動延音
		///	</summary>
		AutoSustain,

		/// <summary>
		/// 電腦控制延音
		///	</summary>
		GameControllingSustain,

	};

	enum class PianoPitchMovement {
		None,
		Raise,
		Lower
	};


	class Piano : public TInstrument<PianoAction> {

		int load();

		int load(OutputManager* o);

	public:

		Piano();

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

		virtual int MoveOctave(PianoPitchMovement m) = 0;

	protected:

		OutputManager* outputManager = nullptr;

		/// <summary>
		/// 目前的延音方式
		///	</summary>
		SustainType sustainType = SustainType::None;

		/// <summary>
		/// 是否有打開自動延音
		///	</summary>
		bool isAutoSustain = false;

		bool isSensitive = true;

		/// <summary>
		/// 看哪個鍵正在按下
		///	</summary>
		map<PianoAction, bool> isPressingMap;

		virtual int update();

		virtual int OnKeyDown(pair<PianoAction, int> action);

		virtual int OnKeyUp(PianoAction action);

		virtual int OnButtonDown(PianoAction action);

		virtual int OnButtonUp(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

		virtual int OnSlide(pair<PianoAction, int> action);
	};

	


}




#endif