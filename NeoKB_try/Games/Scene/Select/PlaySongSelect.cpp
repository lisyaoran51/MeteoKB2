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

	LOG(LogLevel::Debug) << "int PlaySongSelect::onResuming() : go back to song select.";

	/* 在scene::Resume就會把這個child給刪掉了
	DeleteChild(player);
	ThreadMaster::GetInstance().AddObjectToDelete(player);
	player = nullptr;
	*/

	/* 回到選單才能重新選歌 */
	smSelectPanel->SetIsAvailabledForTrigger(true);
	soundSelectPanel->SetIsAvailabledForTrigger(true);

	// 清除上一場遊戲的modifier
	for (int i = 0; i < smSelectPanel->GetSelectedModifiers()->GetValue()->size(); i++) {
		delete smSelectPanel->GetSelectedModifiers()->GetValue()->at(i);
	}
	smSelectPanel->GetSelectedModifiers()->GetValue()->clear();
	

	return MeteoScene::onResuming(lastScene);
}

int PlaySongSelect::updateSheetmusic(WorkingSm * workingSm)
{
	SongSelect::updateSheetmusic(workingSm);

	workingSm->GetModifiers()->BindTo(smSelectPanel->GetSelectedModifiers());


	return 0;
}

int PlaySongSelect::onSelected()
{
	LOG(LogLevel::Info) << "PlaySongSelect::onSelected() : pushing to play scene.";

	/* 進入遊戲後不能再選歌 */
	smSelectPanel->SetIsAvailabledForTrigger(false);
	soundSelectPanel->SetIsAvailabledForTrigger(false);

	Push(player = new PlayerLoader(new Player()));
	return 0;
}

int PlaySongSelect::load()
{

	return 0;
}
