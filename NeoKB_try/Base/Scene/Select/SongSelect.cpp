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
	// �쥻�O��I���A�ڭ̨S�I���ҥH����

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

	// ruleset info �i��ݭn���sbind�@���A����meteo scene�̭���bind�L
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
