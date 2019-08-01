#include "SongSelect.h"


using namespace Base::Scene::Select;


int SongSelect::TriggerOnSelected()
{
	return onSelected();
}

int SongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	// 原本是改背景，我們沒背景所以不用

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
