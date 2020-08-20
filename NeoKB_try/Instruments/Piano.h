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
		/// �⭵�ĦW�٩M�����j�b�@�_
		///	</summary>
		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0);

		/// <summary>
		/// ���J�׶s�M���^�O��j�b�@�_
		///	</summary>
		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0);

		int ChangeSustainType(SustainType sType);

		int ControlSustainPedal(bool down);

		virtual int MoveOctave(PianoPitchMovement m) = 0;

	protected:

		OutputManager* outputManager = nullptr;

		/// <summary>
		/// �ثe�������覡
		///	</summary>
		SustainType sustainType = SustainType::None;

		/// <summary>
		/// �O�_�����}�۰ʩ���
		///	</summary>
		bool isAutoSustain = false;

		bool isSensitive = true;

		/// <summary>
		/// �ݭ����䥿�b���U
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