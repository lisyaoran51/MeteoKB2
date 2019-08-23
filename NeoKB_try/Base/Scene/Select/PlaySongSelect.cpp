#include "PlaySongSelect.h"

#include "../Play/PlayerLoader.h"
#include "../Play/Player.h"




using namespace Base::Scene::Select;


PlaySongSelect::PlaySongSelect() :RegisterType("PlaySongSelect")
{
	registerLoad(bind(static_cast<int(PlaySongSelect::*)(void)>(&PlaySongSelect::load), this));
}

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

int PlaySongSelect::load()
{
	return 0;
}
