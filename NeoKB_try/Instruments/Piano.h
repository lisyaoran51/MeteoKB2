#ifndef PIANO_H
#define PIANO_H

#include "Instrument.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"
#include "Input/PianoAction.h"
#include "../Framework/Output/OutputManager.h"
#include "../Framework/Input/Messages/MessageHandler.h"
#include "../Games/Output/Bluetooths/MeteoBluetoothMessage.h"


using namespace Framework::Input::KeyBindings;
using namespace Instruments::Input;
using namespace Framework::Output;
using namespace Framework::Input::Messages;
using namespace Games::Output::Bluetooths;


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
		/// 這個跟Meteo piano的is game controlling sustain pedal重複了，要小心
		///	</summary>
		GameControllingSustain,

	};

	enum class PianoPitchMovement {
		None,
		Raise,
		Lower
	};


	class Piano : public TInstrument<PianoAction>, public MessageHandler<MeteoBluetoothMessage> {

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

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet);

		/* ------------------- 鋼琴專屬功能 ------------------- */

		virtual int ChangeSustainType(SustainType sType);

		virtual SustainType GetSustainType();

		virtual int ControlSustainPedal(bool down);

		virtual int MoveOctave(PianoPitchMovement m) = 0;

		virtual int SetSensitiveLevel(int sLevel);

		/// <summary>
		/// 在keyboard輸入瞬間立刻執行的on key down，避免鋼琴音有延遲
		///	</summary>
		virtual int OnDirectKeyDown(pair<PianoAction, int> action);

		virtual int OnKeyDown(pair<PianoAction, int> action);

		virtual int OnKeyUp(PianoAction action);

		virtual int OnButtonDown(PianoAction action);

		virtual int OnButtonUp(PianoAction action);

		virtual int OnKnobTurn(pair<PianoAction, int> action);

		virtual int OnSlide(pair<PianoAction, int> action);

	protected:

		OutputManager* outputManager = nullptr;

		/// <summary>
		/// 目前的延音方式
		///	</summary>
		SustainType sustainType = SustainType::None;

		/// <summary>
		/// 是否有打開自動延音 
		/// 跟sustain type重複了，所以拿掉
		///	</summary>
		//bool isAutoSustain = false;

		bool isSensitive = false;

		/// <summary>
		/// 如果按下sensitive，會開幾次根號
		///	</summary>
		int sensitiveLevel = 2;

		/// <summary>
		/// 看哪個鍵正在按下
		///	</summary>
		map<PianoAction, bool> isPressingMap;

		virtual int resetState();

		virtual int update();

		virtual int onMessage(MeteoBluetoothMessage* message);
	};

	


}




#endif