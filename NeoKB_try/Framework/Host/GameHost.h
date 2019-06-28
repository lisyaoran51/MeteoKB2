#ifndef GAME_HOST_H
#define GAME_HOST_H



#include <vector>
#include "../Threading/GameThread.h"
#include "../../Util/DataStructure/Action.h"
#include "../Input/InputEvent.h"
#include "../../Base/Game.h"
#include "../Input/MainInterface.h"
#include "../Input/Handler/InputHandler.h"


using namespace std;
using namespace Framework::Threading;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Base;


namespace Framework {
namespace Host {

	/// <summary>
	/// 遊戲運行的主機
	/// TODO: 繼承IIPCHOST
	/// </summary>
	class GameHost{
	
	public:
		
		GameHost(string name = "");

		int Run(Game game);

		/// <summary>
		/// 讓handler們去註冊，在input thread收到input時，會去用這個叫所有handler，讓每個handler去收自己要的state
		/// </summary>
		Action<int, InputEvent*> OnInput;

	protected:

		FrameBasedClock* sceneGraphClock;

		GameThread* drawThread;

		double maximunDrawHz;

		int drawInitialize();

		int drawFrame();


		GameThread* updateThread;

		double maximunUpdateHz;

		int updateInitialize();

		int updateFrame();


		/// <summary>
		/// 把input丟到main interface裡
		/// </summary>
		GameThread* inputThread;

		double maximunInputHz;

		int inputInitialize();

		int inputFrame();


		int resetInputHandler();

	private :

		FrameworkConfigManager frameworkConfigManager;

		int setupConfig();

		int bootstrapSceneGraph(Game game);

		MainInterface mainInterface;

		ChildAddable dependencies;

		/// <summary>
		/// input manager會成為root，附則把輸入傳給下面的物件
		/// </summary>
		Triggerable* root;

		vector<InputHandler*> availableInputHandler;

		

	};

}}


#endif