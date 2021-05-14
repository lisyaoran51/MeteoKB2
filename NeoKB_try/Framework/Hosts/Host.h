#ifndef HOST_H
#define HOST_H



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

namespace Framework {
namespace Output{
	class OutputManager;
}}


using namespace std;
using namespace Framework::Threading;
using namespace Util::DataStructure;
using namespace Framework::Input;
using namespace Framework::IO;
using namespace Framework::Output;
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
namespace Hosts {

	/// <summary>
	/// �C���B�檺�D��
	/// TODO: �~��IIPCHOST
	/// </summary>
	class Host: public MtoObject {
	
	public:
		
		Host();

		int Initialize(string name = "");

		int Run(Game* game, Instrument* instrument);

		

		/// <summary>
		/// ��handler�̥h���U�A�binput thread����input�ɡA�|�h�γo�ӥs�Ҧ�handler�A���C��handler�h���ۤv�n��state
		/// </summary>
		ActionList<int, InputEvent*> OnInput;

		MainInterface* GetMainInterface();

		vector<InputHandler*>* GetAvailableInputHandlers();

		Storage* GetStorage();

		virtual int SetupOutputManager(OutputManager* oManager) = 0;

	protected:

		bool initialized = false;

		/* IO�� */
		MainInterface* mainInterface;

		virtual int setupMainInterface() = 0;
		
		Storage* storage;

		virtual Storage* getStorage(string sName) = 0;

		virtual int setupConfig();


		/* ��scene graph�� */
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
		/// ��input���main interface��
		/// </summary>
		GameThread* inputThread;

		double maximunInputHz;

		int inputInitialize();

		int inputFrame();

		int resetInputHandlers();

		virtual vector<InputHandler*>* createAvailableInputHandlers() = 0;

		bool exitRequested = false;

		int runLoop();

	private :

		FrameworkConfigManager* frameworkConfigManager;

		int bootstrapSceneGraph(Game* game, Instrument* instrument);

		/// <summary>
		/// input manager�|����root�A���h���J�ǵ��U��������
		/// </summary>
		Container* root = nullptr;

		int iterateSearchDrawable(ChildAddable* r, vector<Drawable*>* drawables);

		Map* canvas;

		vector<InputHandler*>* availableInputHandlers;

		

	};

}}


#endif