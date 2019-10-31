#ifndef GAME_HOST_H
#define GAME_HOST_H



#include <vector>
#include "../Threading/GameThread.h"
#include "../../Util/DataStructure/ActionList.h"
#include "../Input/InputEvent.h"
#include "../Graphic/Drawable.h"
#include "../Allocation/DependencyContainer.h"
#include "../IO/MainInterface.h"
#include "../Configurations/FrameworkConfigManager.h"
#include "../Allocation/Hierachal/Container.h"
#include "../Graphic/Map/Map.h"
#include "../Timing/FrameBasedClock.h"
#include "../IO/MainInterface.h"

namespace Instruments {
	class Instrument;
}
namespace Framework {
	class Game;
}

namespace Framework {
namespace Input {
namespace Handler{
	class InputHandler;
}}}

namespace Framework {
namespace IO{
	class Storage;
}}


using namespace std;
using namespace Framework::Threading;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework;
using namespace Framework::Graphic;
using namespace Framework::Graphic::Maps;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Allocation;
using namespace Framework::Configurations;
using namespace Framework::Timing;
using namespace Instruments;
using namespace Framework::Input::Handler;




namespace Framework {
namespace Host {

	/// <summary>
	/// 遊戲運行的主機
	/// TODO: 繼承IIPCHOST
	/// </summary>
	class GameHost {
	
	public:
		
		GameHost(string name = "");

		int Run(Game* game, Instrument* instrument);

		

		/// <summary>
		/// 讓handler們去註冊，在input thread收到input時，會去用這個叫所有handler，讓每個handler去收自己要的state
		/// </summary>
		ActionList<int, InputEvent*> OnInput;

		MainInterface* GetMainInterface();

		vector<InputHandler*>* GetAvailableInputHandlers();

		Storage* GetStorage();

	protected:

		/* IO用 */
		MainInterface* mainInterface;

		virtual int setupMainInterface() = 0;
		
		Storage* storage;

		virtual Storage* getStorage(string sName) = 0;


		/* 建scene graph用 */
		FrameBasedClock* sceneGraphClock;

		DependencyContainer* dependencies;


		/* draw thread */
		GameThread* drawThread;

		double maximunDrawHz;

		int drawInitialize();

		int drawFrame();


		/* update thread */
		GameThread* updateThread;

		double maximunUpdateHz;

		int updateInitialize();

		int updateFrame();


		/* IO thread */
		/// <summary>
		/// 把input丟到main interface裡
		/// </summary>
		GameThread* inputThread;

		double maximunInputHz;

		int inputInitialize();

		int inputFrame();

		int resetInputHandlers();

		virtual vector<InputHandler*>* createAvailableInputHandlers() = 0;

	private :

		FrameworkConfigManager* frameworkConfigManager;

		int setupConfig();

		int bootstrapSceneGraph(Game* game, Instrument* instrument);

		/// <summary>
		/// input manager會成為root，附則把輸入傳給下面的物件
		/// </summary>
		Container* root;

		int iterateSearchDrawable(ChildAddable* r, vector<Drawable*>* drawables);

		Map* canvas;

		vector<InputHandler*>* availableInputHandlers;

		

	};

}}


#endif