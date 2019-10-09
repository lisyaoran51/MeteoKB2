#ifndef PIANO_H
#define PIANO_H

#include "Instrument.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"

using namespace Framework::Input::KeyBindings;


namespace Instruments {

	enum class SustainType;

	class Piano : public Instrument<PianoAction> {

		int load();

	public:

		Piano();

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

	protected:

		/// <summary>
		/// �ثe�������覡
		///	</summary>
		SustainType sustainType = SustainType::None;

		/// <summary>
		/// �O�_�����}�۰ʩ���
		///	</summary>
		bool isAutoSustain = false;

		/// <summary>
		/// �ݭ����䥿�b���U
		///	</summary>
		map<PianoAction, bool> isPressingMap;

		virtual int OnKeyDown(pair<PianoAction, int> action);

		virtual int OnKeyUp(PianoAction action);

		virtual int OnButtonDown(PianoAction action);

		virtual int OnButtonUp(PianoAction action);

	};

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


}




#endif