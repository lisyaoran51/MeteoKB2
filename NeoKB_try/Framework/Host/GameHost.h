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
	/// �C���B�檺�D��
	/// TODO: �~��IIPCHOST
	/// </summary>
	class GameHost{
	
	public:
		
		GameHost(string name = "");

		int Run(Game game);

		/// <summary>
		/// ��handler�̥h���U�A�binput thread����input�ɡA�|�h�γo�ӥs�Ҧ�handler�A���C��handler�h���ۤv�n��state
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
		/// ��input���main interface��
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
		/// input manager�|����root�A���h���J�ǵ��U��������
		/// </summary>
		Triggerable* root;

		vector<InputHandler*> availableInputHandler;

		

	};

}}


#endif