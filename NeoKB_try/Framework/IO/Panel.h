#ifndef PANEL_H
#define PANEL_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;

namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class Panel : public Peripheral {


	public:

		/// <summary>
		/// �o�ǬO��handler����Ϊ��A�����handler�N�|�۰ʦ���o��panel��X�Ӫ�input
		/// </summary>
		template<class _Type>
		int AddOnButtonDown(_Type* callableObject, function<int(InputState*)> callback, string name);

		template<class _Type>
		int AddOnKnobTurn(_Type* callableObject, function<int(InputState*)> callback, string name);

		template<class _Type>
		int AddOnSliderMove(_Type* callableObject, function<int(InputState*)> callback, string name);

		virtual int ChangeState(int state, bool value) = 0;

	protected:


	private:

		ActionList<int(InputState*)> OnButtonDown;

		ActionList<int(InputState*)> OnKnobTurn;

		ActionList<int(InputState*)> OnSliderMove;


	};


}}









#endif