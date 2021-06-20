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
		/// �S��sustain
		///	</summary>
		None,

		/// <summary>
		/// �ν�O
		///	</summary>
		SustainPedal,

		/// <summary>
		/// �۰ʩ���
		///	</summary>
		AutoSustain,

		/// <summary>
		/// �q�������
		/// �o�Ӹ�Meteo piano��is game controlling sustain pedal���ƤF�A�n�p��
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
		/// �⭵�ĦW�٩M�����j�b�@�_
		///	</summary>
		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0);

		/// <summary>
		/// ���J�׶s�M���^�O��j�b�@�_
		///	</summary>
		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet);

		/* ------------------- ���^�M�ݥ\�� ------------------- */

		virtual int ChangeSustainType(SustainType sType);

		virtual SustainType GetSustainType();

		virtual int ControlSustainPedal(bool down);

		virtual int MoveOctave(PianoPitchMovement m) = 0;

		virtual int SetSensitiveLevel(int sLevel);

		/// <summary>
		/// �bkeyboard��J�����ߨ���檺on key down�A�קK���^��������
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
		/// �ثe�������覡
		///	</summary>
		SustainType sustainType = SustainType::None;

		/// <summary>
		/// �O�_�����}�۰ʩ��� 
		/// ��sustain type���ƤF�A�ҥH����
		///	</summary>
		//bool isAutoSustain = false;

		bool isSensitive = false;

		/// <summary>
		/// �p�G���Usensitive�A�|�}�X���ڸ�
		///	</summary>
		int sensitiveLevel = 2;

		/// <summary>
		/// �ݭ����䥿�b���U
		///	</summary>
		map<PianoAction, bool> isPressingMap;

		virtual int resetState();

		virtual int update();

		virtual int onMessage(MeteoBluetoothMessage* message);
	};

	


}




#endif