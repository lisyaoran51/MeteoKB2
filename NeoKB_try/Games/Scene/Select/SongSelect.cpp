#include "SongSelect.h"

#include "../../MeteoGame.h"
#include "../../../Util/DataStructure/FileSegmentMap.h"
#include <stdio.h>
#include "../../../Util/StringSplitter.h"
#include "../../../Framework/Threading/ThreadMaster.h"


using namespace Games::Scenes::Select;
using namespace Games;
using namespace Util::DataStructure;
using namespace Util;
using namespace Framework::Threading;



SongSelect::SongSelect(): RegisterType("SongSelect")
{

	registerLoad(bind(static_cast<int(SongSelect::*)(void)>(&SongSelect::load), this));
}

int SongSelect::TriggerOnSelected()
{
	return onSelected();
}

int SongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	// 原本是改背景，我們沒背景所以不用

	return 0;
}

int SongSelect::onSelected()
{
	updateSheetmusic(workingSm.GetValue());
	return 0;
}

int SongSelect::load()
{
	SmManager* sManager = GetCache<SmManager>("SmManager");
	if (!sManager)
		throw runtime_error("int SongSelect::load() : SmManager not found in cache.");

	MeteoGame* game = GetCache<MeteoGame>("MeteoGame");
	if (!game)
		throw runtime_error("int SongSelect::load() : MeteoGame not found in cache.");

	Storage* s = GetCache<Storage>("Storage");
	if (!s)
		throw runtime_error("int SongSelect::load() : Storage not found in cache.");

	return load(sManager, game, s);
}

int SongSelect::load(SmManager * sManager, MeteoGame * game, Storage* s)
{
	smManager = sManager;

	storage = s;

	// ruleset info 可能需要重新bind一次，本來meteo scene裡面有bind過
	
	/* 更換音色 */
	soundSelectPanel = new SoundSelectPanel();

	LOG(LogLevel::Debug) << "SongSelect::load() : adding sound panel.";

	AddChild(soundSelectPanel);

	// 把選則歌曲的method註冊進去select panel裡面，select panel選好歌就會直接呼叫回來
	smSelectPanel = new SheetmusicSelectPanel();


	// 這邊會出問題，有沒有可能bind上去的是base function 不是override?
	// 經過實驗，不管有沒有Static_cast，bind上去的都會是override，不會是base
	// 如果一定要用base，可以用lambda式，例如
	// function<int(void)> func = [pointer]() {return pointer->base::VirtualFunction(); };
	smSelectPanel->StartRequest = bind(&SongSelect::onSelected, this);
	smSelectPanel->SelectionChanged = bind(&SongSelect::selectionChanged, this, placeholders::_1);

	smSelectPanel->AddOnDownloadSheetmusicSuccess(this, [=](FileSegmentMap* fSegmentMap) {

		LOG(LogLevel::Debug) << "SongSelect::Lambda_HandleDownloadSheetmusicSuccess() : download [" << fSegmentMap->fileName << "] success.";


		/*
		 * 重要
		 */
		ThreadMaster::GetInstance().SwitchGameStatus((int)GameStatus::Perform);
		/*
		 * 重要
		 */

		if (false) {

			fstream* stream = storage->GetStream(string("temp/") + fSegmentMap->fileName + string(".temp"), FileAccess::Write, FileMode::Create);
			fSegmentMap->WriteFile(stream);
			stream->close();
			delete stream;
			stream = nullptr;

			// 解密、解壓縮
			string decompressCommand = string("tar -xvf ") + storage->GetTempBasePath() + string("/temp/") + fSegmentMap->fileName + string(".temp ")
				+ storage->GetTempBasePath() + string("/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension() + "/" + fSegmentMap->fileName;

			FILE* fp = popen(decompressCommand.c_str(), "r");
			if (fp == NULL) {
				// error
			}
			pclose(fp);

			// 刪除加密檔
			string deleteCommand = string("rm -f ") + storage->GetTempBasePath() + string("/temp/") + fSegmentMap->fileName + string(".temp ");
			fp = popen(deleteCommand.c_str(), "r");
			if (fp == NULL) {
				// error
			}
			pclose(fp);
		}

		FILE* fp = popen((string("mkdir /home/pi/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension()).c_str(), "r");
		if (fp == NULL) {
			LOG(LogLevel::Error) << "SongSelect::Lambda_HandleDownloadSheetmusicSuccess() : fail to mkdir [" << (string("mkdir /home/pi/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension()) << "].";
		}
		pclose(fp);

		fp = popen((string("cp /home/pi/Sheetmusics/") + fSegmentMap->fileName + string(" /home/pi/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension() + string("/")).c_str(), "r");
		if (fp == NULL) {
			LOG(LogLevel::Error) << "SongSelect::Lambda_HandleDownloadSheetmusicSuccess() : fail to cp [" << (string("cp /home/pi/Sheetmusics/") + fSegmentMap->fileName + string(" /home/pi/") + fSegmentMap->GetFileNameWithoutExtension() + string("/")) << "].";

		}
		pclose(fp);

		//string path = storage->GetTempBasePath() + string("/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension();
		string path = string("/home/pi/Sheetmusics/") + fSegmentMap->GetFileNameWithoutExtension();

		LOG(LogLevel::Debug) << "SongSelect::Lambda_HandleDownloadSheetmusicSuccess() : import [" << path << "] to sm manager.";

		vector<string> paths;
		paths.push_back(path);

		if (smManager->Import(&paths) < 0) {
			// throw error
			LOG(LogLevel::Error) << "SongSelect::selectionChanged() : Failed to import new download sheetmusic [" << fSegmentMap->fileName << "].";

			// 丟藍芽錯誤訊號
		}

		fSegmentMap->Erase();

		return 0;

		for (int i = 0; i < smManager->GetSmInfos()->size(); i++) {
			LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : new load song [" << fSegmentMap->fileName << "].";
			if (smManager->GetSmInfos()->at(i)->fileName == fSegmentMap->fileName) {

				LOG(LogLevel::Debug) << "int SheetmusicSelectPanel::load() : song [" << fSegmentMap->fileName << "] found.";

				smSelectPanel->SelectionChanged(smManager->GetSmInfos()->at(i));
				
				smSelectPanel->StartRequest();
			}
		}

		/*
		 * 重要
		 */
		ThreadMaster::GetInstance().SwitchGameStatus((int)GameStatus::Game);
		/*
		 * 重要
		 */

		//smSelectPanel->SelectAndStart(fSegmentMap->GetFileNameWithoutExtension());

		return 0;
	}, "SongSelect::Lambda_HandleDownloadSheetmusicSuccess");

	smSelectPanel->AddOnDownloadSheetmusicFail(this, [=](FileSegmentMap* fSegmentMap) {


		LOG(LogLevel::Debug) << "SongSelect::Lambda_HandleDownloadSheetmusicSuccess() : dounload [" << fSegmentMap->fileName << "] fail.";

		fSegmentMap->Erase();

		/*
		 * 重要
		 */
		ThreadMaster::GetInstance().SwitchGameStatus((int)GameStatus::Perform);
		/*
		 * 重要
		 */

		return 0;
	}, "SongSelect::Lambda_HandleDownloadSheetmusicFail");

	LOG(LogLevel::Debug) << "SongSelect::load() : adding sheetmusic panel.";

	AddChild(smSelectPanel);
	return 0;
}

int SongSelect::selectionChanged(SmInfo * sheetmusicInfo)
{
	LOG(LogLevel::Info) << "SongSelect::selectionChanged() : select [" << sheetmusicInfo->metadata->Title << "].";

	if (workingSm.GetValue() != nullptr) {

		LOG(LogLevel::Depricated) << "SongSelect::selectionChanged() : working sm value = " << workingSm.GetValue();

		if (!workingSm.GetValue()->IsTheSameSm(sheetmusicInfo)) {

			LOG(LogLevel::Depricated) << "SongSelect::selectionChanged() : the same working sm. delete last and recreate.";

			workingSm.SetValue(smManager->GetWorkingSm(sheetmusicInfo), true);

			LOG(LogLevel::Depricated) << "SongSelect::selectionChanged() : test " << workingSm.GetValue()->GetSm();
			// ??需要delete舊的workingSm嗎?
		}
	}
	else {
		LOG(LogLevel::Depricated) << [](BindablePointer<WorkingSm*>* wSm, MeteoScene* scene) {
			for (int i = 0; i < wSm->GetBindings()->size(); i++) {
				LOG(LogLevel::Depricated) << "SongSelect::selectionChanged : working sm bindings [" << i << "] = [" << wSm->GetBindings()->at(i) << "].";
			}

			for (MeteoScene* s = scene; s != nullptr; s = dynamic_cast<MeteoScene*>(s->GetParentScene())) {
				LOG(LogLevel::Depricated) << "SongSelect::selectionChanged : scene [" << s->GetTypeName() << "]'s working sm is [" << s->GetWorkingSm() << "].";
			}
			return 0;
		}(&workingSm, this);

		workingSm.SetValue(smManager->GetWorkingSm(sheetmusicInfo), true);
	}

	// 這邊是如果用手機傳訊息選歌的時候再執行，他會把modifier蓋過去，讓手機可以設定modifier
	// updateSheetmusic(workingSm.GetValue());

	return 0;
}
