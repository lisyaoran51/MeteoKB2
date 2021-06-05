#ifndef METEO_SCENE_H
#define METEO_SCENE_H

#include "../../Framework/Scenes/Scene.h"
#include "../../Util/DataStructure/BindablePointer.h"
#include "../Sheetmusic/WorkingSheetmusic.h"
#include "../Ruleset/RulesetInfo.h"
#include "../../Framework/Input/Messages/MessageHandler.h"
#include "../Output/Bluetooths/MeteoBluetoothMessage.h"
#include "../../Framework/Output/OutputManager.h"



namespace Games {
	class MeteoGame;
	class MeteoGameBase;
}

using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Games::Sheetmusics;
using namespace Games::Rulesets;
using namespace Games;
using namespace Framework::Input::Messages;
using namespace Games::Output::Bluetooths;
using namespace Framework::Output;


namespace Games {
namespace Scenes {

	class MeteoScene : public Scene, public MessageHandler<MeteoBluetoothMessage> {


		int load();

		int load(MeteoGame* game, MeteoGameBase* gameBase, OutputManager* o);


	public:

		BindablePointer<WorkingSm*>* GetWorkingSm();

	protected:

		MeteoScene();

		virtual ~MeteoScene();

		virtual int onExpire();

		virtual int onEntering(Scene* lastScene);

		virtual int onExiting(Scene* lastScene);

		virtual int onSuspending(Scene* lastScene);

		virtual int onResuming(Scene* lastScene);

		/* ----------------------Scene---------------------- */

		BindablePointer<WorkingSm*> workingSm;

		BindablePointer<RulesetInfo*> rulesetInfo;

		OutputManager* outputManager = nullptr;

		virtual int onMessage(MeteoBluetoothMessage* command);

	};


}
}



#endif