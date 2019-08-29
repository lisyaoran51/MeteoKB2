#include "SongSelect.h"


using namespace Base::Scenes::Select;


SongSelect::SongSelect(): RegisterType("SongSelect")
{
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

int SongSelect::load()
{
	SmManager* sManager = GetCache<SmManager>("SmManager");
	if (!sManager)
		throw runtime_error("int SongSelect::load() : SmManager not found in cache.");

	MeteoGame* game = GetCache<MeteoGame>("MeteoGame");
	if (!game)
		throw runtime_error("int SongSelect::load() : MeteoGame not found in cache.");

	return load(sManager, game);
}

int SongSelect::load(SmManager * sManager, MeteoGame * game)
{
	smManager = sManager;

	// ruleset info 可能需要重新bind一次，本來meteo scene裡面有bind過
	return 0;
}

int SongSelect::selectionChanged(SmInfo * sheetmusicInfo)
{

	if (!workingSm.GetValue()->IsTheSameSm(sheetmusicInfo)) {


		workingSm.SetValue(smManager->GetWorkingSm(sheetmusicInfo), true);

	}

	updateSheetmusic(workingSm.GetValue());

	return 0;
}
