#include "SongSelect.h"


using namespace Base::Scene::Select;


int SongSelect::TriggerOnSelected()
{
	return onSelected();
}

int SongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	// �쥻�O��I���A�ڭ̨S�I���ҥH����

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
