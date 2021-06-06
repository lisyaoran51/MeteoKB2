#include "PlaySongSelect.h"

#include "../Play/PlayerLoader.h"
#include "../Play/Player.h"
#include "../../../Framework/Threading/ThreadMaster.h"
#include "../../../RulesetMeteor/Ruleset/Modifiers/MusicGameModifier.h"



using namespace Framework::Threading;
using namespace Games::Scenes::Select;
using namespace Meteor::Rulesets::Modifiers;


PlaySongSelect::PlaySongSelect() :RegisterType("PlaySongSelect")
{
	registerLoad(bind(static_cast<int(PlaySongSelect::*)(void)>(&PlaySongSelect::load), this));
}

int PlaySongSelect::onResuming(Scene * lastScene)
{

	LOG(LogLevel::Debug) << "int PlaySongSelect::onResuming() : go back to song select.";

	/* �bscene::Resume�N�|��o��child���R���F
	DeleteChild(player);
	ThreadMaster::GetInstance().AddObjectToDelete(player);
	player = nullptr;
	*/

	/* �^����~�୫�s��q */
	smSelectPanel->SetIsAvailabledForTrigger(true);
	soundSelectPanel->SetIsAvailabledForTrigger(true);

	// �M���W�@���C����modifier
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


	LOG(LogLevel::Info) << "PlaySongSelect::updateSheetmusic() : updating modifiers.";

	if (workingSm->GetModifiers()->GetValue() == nullptr) {
		// �o����[mod�A����n�����A�\�bon select(on command)
		workingSm->GetModifiers()->SetValue(new vector<Modifier*>());
		//workingSm.GetValue()->GetModifiers()->GetValue()->push_back(new AutoPedalModifier());
		workingSm->GetModifiers()->GetValue()->push_back(new MusicGameModifier());
		//workingSm.GetValue()->GetModifiers()->GetValue()->push_back(new MeteorDifficultyModifier(SmDifficultyDifficulty::Easy));
		//workingSm.GetValue()->GetModifiers()->GetValue()->push_back(new HandModifier(SmDifficultyHandType::Right));
		//workingSm.GetValue()->GetModifiers()->GetValue()->push_back(new RepeatPracticeModifier(2, 2));
		//WhiteKeyTargetLineModifier* modifier = new WhiteKeyTargetLineModifier();
		//modifier->SetValue(10, 0);
		//workingSm.GetValue()->GetModifiers()->GetValue()->push_back(modifier);
	}
	else if(workingSm->GetModifiers()->GetValue()->size() == 0)
		workingSm->GetModifiers()->GetValue()->push_back(new MusicGameModifier());


	return 0;
}

int PlaySongSelect::onSelected()
{
	SongSelect::onSelected();
	LOG(LogLevel::Info) << "PlaySongSelect::onSelected() : pushing to play scene.";


	/* �i�J�C���ᤣ��A��q */
	smSelectPanel->SetIsAvailabledForTrigger(false);
	soundSelectPanel->SetIsAvailabledForTrigger(false);

	Push(player = new PlayerLoader(new Player()));
	return 0;
}

int PlaySongSelect::load()
{

	return 0;
}
