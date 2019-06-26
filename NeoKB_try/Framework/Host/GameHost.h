#ifndef GAME_HOST_H
#define GAME_HOST_H



#include <vector>

using namespace std;


namespace Framework {
namespace Host {

	/// <summary>
	/// 遊戲運行的主機
	/// TODO: 繼承IIPCHOST
	/// </summary>
	class GameHost {
	
	public:
		
		GameHost(String name = "");

		int Run(Game game);

		/// <summary>
		/// 讓handler們去註冊，在input thread收到input時，會去用這個叫所有handler，讓每個handler去收自己要的state
		/// </summary>
		Action<int(InputEvent*)> OnInput;

	protected:

		GameThread DrawThread;

		double MaximunDrawHz;

		int DrawInitialize();

		int DrawFrame();


		GameThread UpdateThread;

		double MaximunUpdateHz;

		int UpdateInitialize();

		int UpdateFrame();


		/// <summary>
		/// 把input丟到main interface裡
		/// </summary>
		GameThread InputThread;

		double MaximunInputHz;

		int InputInitialize();

		int InputFrame();


		int ResetInputHandler();

	private :

		static GameHost instance;

		FrameworkConfigManager frameworkConfigManager;

		int setupConfig();

		int bootstrapSceneGraph(Game game);

		MainInterface mainInterface;

		ChildAddable dependencies;

		/// <summary>
		/// input manager會成為root，附則把輸入傳給下面的物件
		/// </summary>
		ChildAddable root;

		vector<InputHandler*> availableInputHandler;

		

	};

}}


#endif