#include "PlaySongSelect.h"

#include "../Play/PlayerLoader.h"
#include "../Play/Player.h"




using namespace Games::Scenes::Select;


PlaySongSelect::PlaySongSelect() :RegisterType("PlaySongSelect")
{
	registerLoad(bind(static_cast<int(PlaySongSelect::*)(void)>(&PlaySongSelect::load), this));
}

int PlaySongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	SongSelect::updateSheetmusic(workingSm);

	for (int i = 0; i < selectedModifiers.size(); i++)
		workingSm->GetModifiers()->GetValue()->push_back(selectedModifiers[i]);

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
