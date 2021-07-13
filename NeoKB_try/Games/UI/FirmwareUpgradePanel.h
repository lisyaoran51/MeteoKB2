#ifndef FIRMWARE_UPGRADE_PANEL_H
#define FIRMWARE_UPGRADE_PANEL_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../../Framework/Output/OutputManager.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../Framework/IO/Communications/CommunicationAccess.h"
#include "../../Framework/Input/Messages/MessageHandler.h"
#include "../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../Util/DataStructure/FileSegmentMap.h"


using namespace Framework::Output;
using namespace Games::Output::Bluetooths;
using namespace Framework::IO::Communications;
using namespace Framework::Configurations;
using namespace Util::DataStructure;

namespace Games {
namespace UI{

	class FirmwareUpgradePanel : public Container, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(OutputManager* o, CommunicationAccess* c, FrameworkConfigManager* f, Storage* s);

		vector<string> getFileNames(string directoryPath);

	public:

		FirmwareUpgradePanel();

	protected:

		OutputManager* outputManager = nullptr;

		CommunicationAccess* communicationAccess = nullptr;

		Storage* storage = nullptr;


		/// <summary>
		/// �O�_��s�����A�p�G��s�����H��A���D���}�{���A���M�A�]���|���������s����ШD
		/// </summary>
		bool isUpgraded = false;

		/// <summary>
		/// ��s���N����A������L��s�ШD
		/// </summary>
		bool isUpgrading = false;

		/// <summary>
		/// �̫᭱���n�["/"
		/// �U���|����Ӥl��Ƨ��ASplits��Ƨ�����ΡAFiles��Ƨ���X�֧������ɮ�
		/// </summary>
		string firmwareDirectory = "NewFirmwares";

		/// <summary>
		/// �{�b���b�]��firmware�W��
		/// </summary>
		string firmwareName = "Mto00000";

		/// <summary>
		/// �{�b���b�]��firmware�W�١A�ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex��
		/// </summary>
		long firmwareVersion = 0;

		/// <summary>
		/// �ثe�{�����Ϊ�����W�١A�ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex��
		/// </summary>
		string tempFirmwareSplitName = "";

		/// <summary>
		/// �ثe�{�����Ϊ�����W�١A�ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex�ȡA�Y������s��
		/// </summary>
		long tempFirmwareSplitVersion = 0;
		
		int tempFirmwareSplitCount = 0;

		/// <summary>
		/// �Y�N�U�����s����W�١A�ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex��
		/// </summary>
		string newFirmwareName = "";

		/// <summary>
		/// �Y�N�U�����s����W�٥[�W.(split)�A�ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex��
		/// </summary>
		string newFirmwareSplitName = "";

		int maxNewFirmwareSplitCount = 0;

		/// <summary>
		/// ���i�H��vector�A�u�Omap.find�ܤ�K�A�ҥH�N��map
		/// </summary>
		map<int, string> newFirmwareSplits;

		virtual int handleOnRequestSplitFail(FileSegmentMap* fSegmentMap);

		virtual int handleOnRequestSplitSuccess(FileSegmentMap* fSegmentMap);

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}}




#endif