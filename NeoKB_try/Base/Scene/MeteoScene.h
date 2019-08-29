#ifndef METEO_SCENE_H
#define METEO_SCENE_H

#include "../../Framework/Scenes/Scene.h"
#include "../../Util/DataStructure/Bindable.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../Ruleset/RulesetInfo.h"
#include "../MeteoGame.h"
#include "../MeteoGameBase.h"


using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Base::Sheetmusics;
using namespace Base::Rulesets;
using namespace Base;


namespace Base {
namespace Scenes {

	class MeteoScene : public Scene {


		int load();

		int load(MeteoGame* game, MeteoGameBase* gameBase);

	public:



	protected:

		MeteoScene();

		~MeteoScene();

		Bindable<WorkingSm*> workingSm;

		Bindable<RulesetInfo*> rulesetInfo;


	};


}
}



#endif