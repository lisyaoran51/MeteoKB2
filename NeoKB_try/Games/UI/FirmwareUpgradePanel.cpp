#include "FirmwareUpgradePanel.h"

#include <experimental/filesystem>
#include "../../Util/StringSplitter.h"
#include "../IO/Communications/BackgroundGetBinaryBleRequest.h"
#include "../../Util/FileSplitCombiner.h"
#include <stdio.h>


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

	Storage * s = GetCache<Storage>("Storage");
	if (!s)
		throw runtime_error("int FirmwareUpgradePanel::load : Storage not found in cache.");

	return load(o, c, f, s);
}

int FirmwareUpgradePanel::load(OutputManager * o, CommunicationAccess * c, FrameworkConfigManager* f, Storage* s)
{
	LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : start reading splits.";
	outputManager = o;
	communicationAccess = c;
	storage = s;

	firmwareName = MTO_VERSION;
	try {
		if (firmwareName.length() == 8){
			firmwareVersion = stol(string("0x") + firmwareName.substr(3, 5), nullptr, 16);
			LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : temp FirmwareVersion is [" << hex << firmwareVersion << "].";
		}
		else {

			LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : fail to get temp FirmwareVersion.";
			// TODO: 傳錯誤訊息
		}
	}
	catch (exception& e) {
		// TODO: 傳錯誤訊息，要求系統還原
		LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : firmware version error. reset firmware";
		firmwareVersion = -1;
	}


	// f->Get firmware directory

	vector<string>* splitFileNames = storage->GetFileNames(firmwareDirectory + string("/Splits"));

	// 檔案名稱為MtoXXXXX，XXXXX為五位數的hex值
	long maxSplitVersion = 0;

	for (int i = 0; i < splitFileNames->size(); i++) {

		string splitFileName = StringSplitter::Split(splitFileNames->at(i), "/").back();

		if (splitFileName.length() < 8)
			continue;

		LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : split [" << i << "] read." << splitFileName;

		string tempVersionHex = splitFileName.substr(3, 5);
		long tempVersion = stol(string("0x") + tempVersionHex, nullptr, 16);
		if (tempVersion > maxSplitVersion)
			maxSplitVersion = tempVersion;

	}

	tempFirmwareSplitVersion = maxSplitVersion;

	tempFirmwareSplitCount = 0;

	/* 檢查目前的split有哪些 */
	for (int i = 0; i < splitFileNames->size(); i++) {

		string splitFileName = StringSplitter::Split(splitFileNames->at(i), "/").back();

		if (splitFileName.length() < 8)
			continue;

		LOG(LogLevel::Debug) << "FirmwareUpgradePanel::load() : split [" << i << "] check." << splitFileName;

		string tempVersionHex = splitFileName.substr(3, 5);
		long tempVersion = stol(string("0x") + tempVersionHex, nullptr, 16);
		if (tempVersion == tempFirmwareSplitVersion) {

			if (splitFileName.length() < 9)
				continue;

			string tempSplitDec = splitFileName.substr(9, splitFileName.length() - 9);
			newFirmwareSplits[stoi(tempSplitDec)] = splitFileName;
			tempFirmwareSplitCount++;
		}

	}

	// TODO: 清除舊的split

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
	isInputable = true;

	registerLoad(bind(static_cast<int(FirmwareUpgradePanel::*)(void)>(&FirmwareUpgradePanel::load), this));
}

int FirmwareUpgradePanel::handleOnRequestSplitFail()
{

	GetScheduler()->AddDelayedTask([=]() {
		LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitFail() : request failed. resend request. ";

		int firstSplitToGet = -1;
		for (int i = 0; i < maxNewFirmwareSplitCount; i++) {
			if (newFirmwareSplits.find(i) == newFirmwareSplits.end()) {
				firstSplitToGet = i;
				break;
			}
		}

		if (firstSplitToGet == -1) {
			LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitFail() : request failed. resend request but found all splits ready. ";
			// TODO:例外狀況，如果本來就有所有split，就直接重組就好了
			return 0;
		}


		MeteoContextBluetoothMessage* requestMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestNewFirmwareSplit);
		json context;
		context["FileName"] = newFirmwareName + string(".") + to_string(firstSplitToGet);


		BackgroundGetBinaryBleRequest* getFirmwareBleRequest = new BackgroundGetBinaryBleRequest{
			firmwareDirectory + string("/Splits/") + newFirmwareName + string(".") + to_string(firstSplitToGet),
			requestMessage,
			MeteoCommand::AckRequestNewFirmwareSplit,
			MeteoCommand::NewFirmwareSplitFileSegment,
			MeteoCommand::AckNewFirmwareSplitFileSegment,
			MeteoCommand::FinishWriteNewFirmwareSplit,
			MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment,
			MeteoCommand::AckFinishWriteNewFirmwareSplit
		};

		getFirmwareBleRequest->AddOnFail(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitFail, this), "FirmwareUpgradePanel::handleOnRequestSplitFail");

		getFirmwareBleRequest->AddOnSuccess(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitSuccess, this, placeholders::_1), "FirmwareUpgradePanel::handleOnRequestSplitSuccess");


		communicationAccess->Queue(getFirmwareBleRequest);

		return 0;
	}, 0.1);


	return 0;
}

int FirmwareUpgradePanel::handleOnRequestSplitSuccess(FileSegmentMap* fSegmentMap)
{

	LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitSuccess() : download [" << fSegmentMap->fileName << "] success.";

	GetScheduler()->AddDelayedTask([=]() {
		LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitSuccess() : request success. find next split request. ";

		int firstSplitToGet = -1;
		for (int i = 0; i < maxNewFirmwareSplitCount; i++) {
			if (newFirmwareSplits.find(i) == newFirmwareSplits.end()) {
				firstSplitToGet = i;
				break;
			}
		}

		if (firstSplitToGet == -1) {
			LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitSuccess() : all splits ready. start combination.";

			vector<string> splitPaths;
			for (int i = 0; i < maxNewFirmwareSplitCount; i++) {
				splitPaths.push_back(storage->GetBasePath() + string("/") + firmwareDirectory + string("/Splits/") + newFirmwareName + string(".") + to_string(i));
			}

			if (FileSplitCombiner::Combine(storage->GetBasePath() + string("/") + firmwareDirectory + string("/Files/") + newFirmwareName, splitPaths) >= 0) {

				LOG(LogLevel::Debug) << "FirmwareUpgradePanel::handleOnRequestSplitSuccess() : combination successed. new firmware will launch on next start.";

				MeteoContextBluetoothMessage* finishMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinishRequestNewFirmware);
				json context;

				context["FileName"] = newFirmwareName;

				finishMessage->SetContextInJson(context);
				finishMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

				outputManager->PushMessage(finishMessage);


				isUpgraded = true;
				isUpgrading = false;

			}
			else {
				LOG(LogLevel::Warning) << "FirmwareUpgradePanel::handleOnRequestSplitSuccess() : combination failed. restart upgrade";

				tempFirmwareSplitCount = 0;
				newFirmwareSplits.clear();

				MeteoContextBluetoothMessage* requestMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestNewFirmwareSplit);
				json context;
				context["FileName"] = newFirmwareName + string(".0");

				requestMessage->SetContextInJson(context);
				requestMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);


				BackgroundGetBinaryBleRequest* getFirmwareBleRequest = new BackgroundGetBinaryBleRequest{
					firmwareDirectory + string("/Splits/") + newFirmwareName + string(".0"),
					requestMessage,
					MeteoCommand::AckRequestNewFirmwareSplit,
					MeteoCommand::NewFirmwareSplitFileSegment,
					MeteoCommand::AckNewFirmwareSplitFileSegment,
					MeteoCommand::FinishWriteNewFirmwareSplit,
					MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment,
					MeteoCommand::AckFinishWriteNewFirmwareSplit
				};

				getFirmwareBleRequest->AddOnFail(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitFail, this), "FirmwareUpgradePanel::handleOnRequestSplitFail");

				getFirmwareBleRequest->AddOnSuccess(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitSuccess, this, placeholders::_1), "FirmwareUpgradePanel::handleOnRequestSplitSuccess");


				communicationAccess->Queue(getFirmwareBleRequest);

			}

			/* 刪除split */
			tempFirmwareSplitCount = 0;
			newFirmwareSplits.clear();

			string deleteCommand = string("rm -f ") + storage->GetBasePath() + string("/") + firmwareDirectory + string("/Splits/*");
			FILE* fp = popen(deleteCommand.c_str(), "r");
			if (fp == NULL) {
				// error
			}
			pclose(fp);

			return 0;
		}


		MeteoContextBluetoothMessage* requestMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestNewFirmwareSplit);
		json context;
		context["FileName"] = newFirmwareName + string(".") + to_string(firstSplitToGet);

		requestMessage->SetContextInJson(context);
		requestMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);


		BackgroundGetBinaryBleRequest* getFirmwareBleRequest = new BackgroundGetBinaryBleRequest{
			firmwareDirectory + string("/Splits/") + newFirmwareName + string(".") + to_string(firstSplitToGet),
			requestMessage,
			MeteoCommand::AckRequestNewFirmwareSplit,
			MeteoCommand::NewFirmwareSplitFileSegment,
			MeteoCommand::AckNewFirmwareSplitFileSegment,
			MeteoCommand::FinishWriteNewFirmwareSplit,
			MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment,
			MeteoCommand::AckFinishWriteNewFirmwareSplit
		};

		getFirmwareBleRequest->AddOnFail(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitFail, this), "FirmwareUpgradePanel::handleOnRequestSplitFail");

		getFirmwareBleRequest->AddOnSuccess(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitSuccess, this, placeholders::_1), "FirmwareUpgradePanel::handleOnRequestSplitSuccess");


		communicationAccess->Queue(getFirmwareBleRequest);

		return 0;
	}, 0.1);

	fSegmentMap->Erase();

	return 0;
}

int FirmwareUpgradePanel::onMessage(MeteoBluetoothMessage * message)
{
	MeteoContextBluetoothMessage* contextMessage = dynamic_cast<MeteoContextBluetoothMessage*>(message);

	if (contextMessage == nullptr) {
		return -1;
	}
	json context = contextMessage->GetContextInJson();

	LOG(LogLevel::Debug) << "FirmwareUpgradePanel::onMessage() : got new bt message. ";

	if (contextMessage->GetCommand() == MeteoCommand::NewFirmwareData) {

		if (firmwareVersion == -1) {
			// 要求還原主程式

			return -1;
		}

		if (isUpgraded || isUpgrading)
			return -1;

		try {

			LOG(LogLevel::Debug) << "FirmwareUpgradePanel::onMessage() : get new firmware data";
			if (context.contains("FileName") == 0 ||
				context.contains("Split") == 0 ||
				context.contains("Checksum") == 0)
				return -1;

			string fileName = context["FileName"].get<string>();
			int split = context["Split"].get<int>();

			long newVersion = stol(string("0x") + fileName.substr(3, 5));

			if (newVersion >= tempFirmwareSplitVersion && newVersion > firmwareVersion) {

				newFirmwareName = fileName;

				maxNewFirmwareSplitCount = split;
				if (tempFirmwareSplitVersion < newVersion) {
					tempFirmwareSplitVersion = newVersion;
					tempFirmwareSplitCount = 0;
				}

				LOG(LogLevel::Debug) << "FirmwareUpgradePanel::onMessage() : new firmware data - filename [" << fileName << "], version [" << hex << newVersion << "], split [" << maxNewFirmwareSplitCount << "], temp already exist split count [" << maxNewFirmwareSplitCount << "].";


				// 檢查目前的firmware有哪些split
				newFirmwareSplits.clear();
				vector<string>* splitFileNames = storage->GetFileNames(firmwareDirectory + string("/Splits"));
				for (int i = 0; i < splitFileNames->size(); i++) {

					string splitFileName = StringSplitter::Split(splitFileNames->at(i), "/").back();

					if (splitFileName.substr(0, 8) != fileName)
						continue;

					if (splitFileName.length() < 9)
						continue;

					int thisSplit = stoi(splitFileName.substr(9, splitFileName.length() - 9));
					newFirmwareSplits[thisSplit] = splitFileName;
				}

				// 檢查目前的firmware缺哪些split
				int firstSplitToGet = -1;
				for (int i = 0; i < maxNewFirmwareSplitCount; i++) {
					if (newFirmwareSplits.find(i) == newFirmwareSplits.end()) {
						firstSplitToGet = i;
						break;
					}
				}

				if (firstSplitToGet == -1) {
					LOG(LogLevel::Debug) << "FirmwareUpgradePanel::onMessage() : [NewFirmwareData] already have all splits. ";
					// TODO:例外狀況，如果本來就有所有split，就直接重組就好了
					return 0;
				}

				MeteoContextBluetoothMessage* requestMessage = new MeteoContextBluetoothMessage(MeteoCommand::RequestNewFirmwareSplit);
				json context;
				context["FileName"] = fileName + string(".") + to_string(firstSplitToGet);

				requestMessage->SetContextInJson(context);
				requestMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

				BackgroundGetBinaryBleRequest* getFirmwareBleRequest = new BackgroundGetBinaryBleRequest{
					firmwareDirectory + string("/Splits/") + fileName + string(".") + to_string(firstSplitToGet),
					requestMessage,
					MeteoCommand::AckRequestNewFirmwareSplit,
					MeteoCommand::NewFirmwareSplitFileSegment,
					MeteoCommand::AckNewFirmwareSplitFileSegment,
					MeteoCommand::FinishWriteNewFirmwareSplit,
					MeteoCommand::RequestRewriteNewFirmwareSplitFileSegment,
					MeteoCommand::AckFinishWriteNewFirmwareSplit
				};

				getFirmwareBleRequest->AddOnFail(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitFail, this), "FirmwareUpgradePanel::handleOnRequestSplitFail");

				getFirmwareBleRequest->AddOnSuccess(this, bind(&FirmwareUpgradePanel::handleOnRequestSplitSuccess, this, placeholders::_1), "FirmwareUpgradePanel::handleOnRequestSplitSuccess");

				communicationAccess->Queue(getFirmwareBleRequest);

				isUpgrading = true;
			}

		}
		catch (exception& e) {
			LOG(LogLevel::Warning) << "FirmwareUpgradePanel::onMessage() : [NewFirmwareData] fail to start request. ";
		}

	}
}
