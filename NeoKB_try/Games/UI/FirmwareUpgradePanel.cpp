#include "FirmwareUpgradePanel.h"

#include <experimental/filesystem>
#include "../../Util/StringSplitter.h"
#include "../IO/Communications/BackgroundGetBinaryBleRequest.h"

#ifndef MTO_VERSION
#define MTO_VERSION "Mto00000"
#endif


using namespace Games::UI;
using namespace std::experimental::filesystem;
using namespace Util;
using namespace Games::IO::Communications;


int FirmwareUpgradePanel::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("int FirmwareUpgradePanel::load : OutputManager not found in cache.");

	CommunicationAccess * c = GetCache<CommunicationAccess>("CommunicationAccess");
	if (!c)
		throw runtime_error("int FirmwareUpgradePanel::load : CommunicationAccess not found in cache.");

	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	if (!f)
		throw runtime_error("int FirmwareUpgradePanel::load : FrameworkConfigManager not found in cache.");

	return load(o, c, f);
}

int FirmwareUpgradePanel::load(OutputManager * o, CommunicationAccess * c, FrameworkConfigManager* f)
{
	outputManager = o;
	communicationAccess = c;

	firmwareName = MTO_VERSION;
	try {
		if (firmwareName.length() > 8)
			firmwareVersion = stol(string("0x") + firmwareName.substr(3, 5));
		else {
			// TODO: �ǿ��~�T��
		}
	}
	catch (exception& e) {
		// TODO: �ǿ��~�T���A�n�D�t���٭�
		firmwareVersion = -1;
	}


	// f->Get firmware directory

	vector<string> splitFileNames = getFileNames(firmwareDirectory + string("/Splits"));

	// �ɮצW�٬�MtoXXXXX�AXXXXX������ƪ�hex��
	long maxSplitVersion = 0;

	for (int i = 0; i < splitFileNames.size(); i++) {

		if (splitFileNames[i].length() < 8)
			continue;
		string tempVersionHex = splitFileNames[i].substr(3, 5);
		long tempVersion = stol(string("0x") + tempVersionHex, nullptr, 0);
		if (tempVersion > maxSplitVersion)
			tempVersion = maxSplitVersion;

	}

	tempFirmwareSplitVersion = maxSplitVersion;

	tempFirmwareSplitCount = 0;

	for (int i = 0; i < splitFileNames.size(); i++) {

		if (splitFileNames[i].length() < 8)
			continue;
		string tempVersionHex = splitFileNames[i].substr(3, 5);
		long tempVersion = stol(string("0x") + tempVersionHex, nullptr, 0);
		if (tempVersion == tempFirmwareSplitVersion)
			tempFirmwareSplitCount++;

	}

	// TODO: �M���ª�split

	return 0;
}

vector<string> FirmwareUpgradePanel::getFileNames(string directoryPath)
{
	try {

		path dPath = directoryPath;
		if (!exists(dPath)) {
			LOG(LogLevel::Error) << "FirmwareUpgradePanel::getFileNames() : not directory [" << directoryPath << "].";
			return vector<string>();
		}

		vector<string> names;

		for (auto& p : directory_iterator(dPath)) {

			if (is_regular_file(p.path())) {

				string fileName = p.path().string();

				fileName = StringSplitter::Split(fileName, "/").back();

				names.push_back(fileName);
			}

		}

		return names;

	}
	catch (exception& e) {

		LOG(LogLevel::Error) << "FirmwareUpgradePanel::getFileNames() : fail to get files under [" << directoryPath << "]." << e.what();

		return vector<string>();
	}

	return vector<string>();
}

FirmwareUpgradePanel::FirmwareUpgradePanel() : RegisterType("FirmwareUpgradePanel")
{
	registerLoad(bind(static_cast<int(FirmwareUpgradePanel::*)(void)>(&FirmwareUpgradePanel::load), this));
}

int FirmwareUpgradePanel::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}
	json context = contextMessage->GetContextInJson();

	LOG(LogLevel::Depricated) << "FirmwareUpgradePanel::onMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::NewFirmwareData) {

		if (firmwareVersion == -1) {
			// �n�D�٭�D�{��

			return -1;
		}

		try {

			string fileName = context["FileName"].get<string>();
			int split = context["Split"].get<int>();

			long newVersion = stol(string("0x") + fileName.substr(3, 5));
			long thisFirmwareVersion = stol(string("0x") + fileName.substr(3, 5));

			if (newVersion >= tempFirmwareSplitVersion && newVersion > firmwareVersion) {

				maxNewFirmwareSplitCount = split;
				if (tempFirmwareSplitVersion < newVersion) {
					tempFirmwareSplitVersion = newVersion;
					tempFirmwareSplitCount = 0;
				}

				// �ˬd�ثe��firmware������split
				newFirmwareSplits.clear();
				vector<string> splitFileNames = getFileNames(firmwareDirectory + string("/Splits"));
				for (int i = 0; i < splitFileNames.size(); i++) {

					if (splitFileNames[i].substr(0, 8) != fileName)
						continue;

					int thisSplit = stoi(splitFileNames[i].substr(9, splitFileNames[i].length() - 9));
					newFirmwareSplits[thisSplit] = splitFileNames[i];

				}

				// �ˬd�ثe��firmware�ʭ���split
				int firstSplitToGet = -1;
				for (int i = 0; i < maxNewFirmwareSplitCount; i++) {
					if (newFirmwareSplits.find(i) == newFirmwareSplits.end()) {
						firstSplitToGet = i;
						break;
					}
				}

				if (firstSplitToGet == -1) {
					// TOFO:�ҥ~���p�A�p�G���ӴN���Ҧ�split�A�N�������մN�n�F
					return 0;
				}

				MeteoContextBluetoothMessage* requestMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestNewFirmwareSplit);
				json context;
				context["FileName"] = fileName + string(".") + to_string(firstSplitToGet);


				BackgroundGetBinaryBleRequest* getFirmwareBleRequest = new BackgroundGetBinaryBleRequest{
					firmwareDirectory + string("/Splits/") + fileName + string(".") + to_string(firstSplitToGet),
					requestMessage,
					MeteoCommand::AckRequestNewFirmwareSplit,
					MeteoCommand::NewFirmwareSplitFileSegment,
					MeteoCommand::AckNewFirmwareSplitFileSegment,
					MeteoCommand::FinishWriteNewFirmwareSplit,
					MeteoCommand::RequestRewriteNewFirmwareSplit,
					MeteoCommand::AckFinishWriteNewFirmwareSplit
				};

				getFirmwareBleRequest->AddOnFail(this, [=](FileSegmentMap* fSegmentMap) {

					// TODO: ���ѴN����
					return 0;
				}, "Lambda_FirmwareUpgradePanel::HandleRequestNewFirmwareSplitFail");

				getFirmwareBleRequest->AddOnSuccess(this, [=](FileSegmentMap* fSegmentMap) {

					// TODO: ���\�N�ǤU�@�өέ���
					return 0;
				}, "Lambda_FirmwareUpgradePanel::HandleRequestNewFirmwareSplitFail");

			}

		}
		catch (exception& e) {

		}

	}
}
