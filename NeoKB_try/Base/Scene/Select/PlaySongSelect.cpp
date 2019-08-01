#include "PlaySongSelect.h"


using namespace Base::Scene::Select;


int PlaySongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	SongSelect::updateSheetmusic(workingSm);

	workingSm->GetModifier()->Set(selectedModifiers);

	return 0;
}

int PlaySongSelect::onSelected()
{

	Push(player = new PlayerLoader(new Player()));
	return 0;
}
