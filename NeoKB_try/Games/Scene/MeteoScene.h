#ifndef METEO_SCENE_H
#define METEO_SCENE_H

#include "../../Framework/Scenes/Scene.h"
#include "../../Util/DataStructure/BindablePointer.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../Ruleset/RulesetInfo.h"



namespace Games {
	class MeteoGame;
	class MeteoGameBase;
}

using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Games::Sheetmusics;
using namespace Games::Rulesets;
using namespace Games;


namespace Games {
namespace Scenes {

	class MeteoScene : public Scene {


		int load();

		int load(MeteoGame* game, MeteoGameBase* gameBase);

	public:

	protected:

		MeteoScene();

		~MeteoScene();

		BindablePointer<WorkingSm*> workingSm;

		BindablePointer<RulesetInfo*> rulesetInfo;


	};


}
}



#endif