#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H


#include "OutputMessage.h"
#include "OutputCollectionManager.h"
#include "../IO/MainInterface.h"
#include "Bluetooths/BluetoothManager.h"
#include "Panels/PanelManager.h"



using namespace Framework::Output::Bluetooths;
using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Framework {
namespace Output{

	class OutputManager : public OutputCollectionManager<OutputComponent> , public MtoObject {

		

	public:

		OutputManager(MainInterface* mInterface);

		//BluetoothManager* GetBluetoothManager();

		//PanelManager* GetPanelManager();

		/// <summary>
		/// 必須在Add item時執行，不然會沒有功能
		/// </summary>
		virtual int SetupPeripheral(MainInterface* mainInterface);

		virtual int PushMessage(OutputMessage* outputMessage);

		virtual int AddItem(OutputComponent* item);

	protected:

		MainInterface* mainInterface = nullptr;

		BluetoothManager* bluetoothManager = nullptr;

		PanelManager* panelManager = nullptr;


	};

}}







#endif
