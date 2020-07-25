#include "PlaySongSelect.h"

#include "../Play/PlayerLoader.h"
#include "../Play/Player.h"
#include "../../../Framework/Threading/ThreadMaster.h"



using namespace Framework::Threading;
using namespace Games::Scenes::Select;


PlaySongSelect::PlaySongSelect() :RegisterType("PlaySongSelect")
{
	registerLoad(bind(static_cast<int(PlaySongSelect::*)(void)>(&PlaySongSelect::load), this));
}

int PlaySongSelect::onResuming(Scene * lastScene)
{

	DeleteChild(player);
	ThreadMaster::GetInstance().AddObjectToDelete(player);
	player = nullptr;


	return 0;
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
	LOG(LogLevel::Info) << "PlaySongSelect::onSelected() : pushing to play scene.";

	Push(player = new PlayerLoader(new Player()));
	return 0;
}

int PlaySongSelect::load()
{


	return 0;
}
