#include "SongSelect.h"

#include "../../MeteoGame.h"
#include "../../../RulesetMeteor/Ruleset/Modifiers/AutoPedalModifier.h"


using namespace Games::Scenes::Select;
using namespace Games;
using namespace Meteor::Rulesets::Modifiers;


SongSelect::SongSelect(): RegisterType("SongSelect")
{

	registerLoad(bind(static_cast<int(SongSelect::*)(void)>(&SongSelect::load), this));
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

	// ���h�q����method���U�i�hselect panel�̭��Aselect panel��n�q�N�|�����I�s�^��
	smSelectPanel = new SheetmusicSelectPanel();


	// �o��|�X���D�A���S���i��bind�W�h���Obase function ���Ooverride?
	// �g�L����A���ަ��S��Static_cast�Abind�W�h�����|�Ooverride�A���|�Obase
	// �p�G�@�w�n��base�A�i�H��lambda���A�Ҧp
	// function<int(void)> func = [pointer]() {return pointer->base::VirtualFunction(); };
	smSelectPanel->StartRequest = bind(&SongSelect::onSelected, this);
	smSelectPanel->SelectionChanged = bind(&SongSelect::selectionChanged, this, placeholders::_1);

	AddChild(smSelectPanel);
	return 0;
}

int SongSelect::selectionChanged(SmInfo * sheetmusicInfo)
{
	LOG(LogLevel::Info) << "SongSelect::selectionChanged() : select [" << sheetmusicInfo->metadata->Title << "].";

	if (workingSm.GetValue() != nullptr) {

		LOG(LogLevel::Debug) << "SongSelect::selectionChanged() : working sm value = " << workingSm.GetValue();

		if (!workingSm.GetValue()->IsTheSameSm(sheetmusicInfo)) {


			workingSm.SetValue(smManager->GetWorkingSm(sheetmusicInfo), true);

			// ??�ݭndelete�ª�workingSm��?
		}
	}
	else {
		LOG(LogLevel::Debug) << [](BindablePointer<WorkingSm*>* wSm, MeteoScene* scene) {
			for (int i = 0; i < wSm->GetBindings()->size(); i++) {
				LOG(LogLevel::Debug) << "SongSelect::selectionChanged : working sm bindings [" << i << "] = [" << wSm->GetBindings()->at(i) << "].";
			}

			for (MeteoScene* s = scene; s != nullptr; s = dynamic_cast<MeteoScene*>(s->GetParentScene())) {
				LOG(LogLevel::Debug) << "SongSelect::selectionChanged : scene [" << s->GetTypeName() << "]'s working sm is [" << s->GetWorkingSm() << "].";
			}
			return 0;
		}(&workingSm, this);

		workingSm.SetValue(smManager->GetWorkingSm(sheetmusicInfo), true);

		// �o����[mod�A����n�����A�\�bon select(on command)
		workingSm.GetValue()->GetModifiers()->GetValue()->push_back(new AutoPedalModifier());
	}

	updateSheetmusic(workingSm.GetValue());

	return 0;
}
