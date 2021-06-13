#ifndef FIRMWARE_UPGRADE_PANEL_H
#define FIRMWARE_UPGRADE_PANEL_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../../Framework/Output/OutputManager.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Framework/IO/Communications/CommunicationAccess.h"
#include "../../Framework/Input/Messages/MessageHandler.h"
#include "../../Framework/Configurations/FrameworkConfigManager.h"


using namespace Framework::Output;
using namespace Games::Output::Bluetooths;
using namespace Framework::IO::Communications;
using namespace Framework::Configurations;

namespace Games {
namespace UI{

	class FirmwareUpgradePanel : public Container, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(OutputManager* o, CommunicationAccess* c, FrameworkConfigManager* f);

		vector<string> getFileNames(string directoryPath);

	public:

		FirmwareUpgradePanel();

	protected:

		OutputManager* outputManager = nullptr;

		CommunicationAccess* communicationAccess = nullptr;

		/// <summary>
		/// 絕對路徑，最後面不要加"/"
		/// </summary>
		string firmwareDirectory = "/NewFirmwares";

		/// <summary>
		/// 現在正在跑的firmware名稱
		/// </summary>
		string firmwareName = "Mto00000";

		/// <summary>
		/// 現在正在跑的firmware名稱，檔案名稱為MtoXXXXX，XXXXX為五位數的hex值
		/// </summary>
		long firmwareVersion = 0;

		/// <summary>
		/// 目前現有分割的韌體名稱，檔案名稱為MtoXXXXX，XXXXX為五位數的hex值
		/// </summary>
		string tempFirmwareSplitName = "";

		/// <summary>
		/// 目前現有分割的韌體名稱，檔案名稱為MtoXXXXX，XXXXX為五位數的hex值，即為韌體編號
		/// </summary>
		long tempFirmwareSplitVersion = 0;
		
		int tempFirmwareSplitCount = 0;

		/// <summary>
		/// 即將下載的新任體名稱，檔案名稱為MtoXXXXX，XXXXX為五位數的hex值
		/// </summary>
		string newFirmwareSplitName = "";

		int maxNewFirmwareSplitCount = 0;

		/// <summary>
		/// 其實可以用vector，只是map.find很方便，所以就用map
		/// </summary>
		map<int, string> newFirmwareSplits;

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}}




#endif