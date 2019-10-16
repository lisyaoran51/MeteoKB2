#ifndef PANEL_H
#define PANEL_H



#include <vector>
#include "Peripheral.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputState.h"
#include "../Devices/PanelDevice.h"

using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::Devices;

namespace Framework {
namespace IO {

	/// <summary>
	/// 抓取輸入的地方，書出的地方
	/// </summary>
	class Panel : public Peripheral {


	public:

		virtual int SetDevice(Device* device);

		virtual int TriggerOnInput();

		/// <summary>
		/// 這些是給handler住測用的，住測完handler就會自動收到這個panel丟出來的input
		/// </summary>
		template<class _Type>
		int AddOnPanelEvent(_Type* callableObject, function<int(InputState*)> callback, string name);

		// 忘記這個要幹嘛了
		//virtual int ChangeState(int state, bool value) = 0;

	protected:

		PanelDevice* matchedPanelDevice;

	private:

		ActionList<int(InputState*)> OnPanelEvent;


	};


}}









#endif