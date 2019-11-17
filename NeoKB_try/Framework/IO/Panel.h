#ifndef PANEL_H
#define PANEL_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"

namespace Framework {
namespace Devices{
	class Device;
	class PanelDevice;
}}

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::Devices;

namespace Framework {
namespace IO {

	/// <summary>
	/// �����J���a��A�ѥX���a��
	/// </summary>
	class Panel : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		/// <summary>
		/// �o�ǬO��handler����Ϊ��A�����handler�N�|�۰ʦ���o��panel��X�Ӫ�input
		/// </summary>
		template<class _Type>
		int AddOnPanelEvent(_Type* callableObject, function<int(InputState*)> callback, string name = "HandlePanelEvent") {
			LOG(LogLevel::Fine) << "Panel::AddOnPanelEvent() : adding handler.";

			OnPanelEvent.Add(callableObject, callback, name);
			return 0;
		}

		// �ѰO�o�ӭn�F���F
		//virtual int ChangeState(int state, bool value) = 0;

	protected:

		PanelDevice* matchedPanelDevice;

	private:

		ActionList<int, InputState*> OnPanelEvent;


	};


}}









#endif