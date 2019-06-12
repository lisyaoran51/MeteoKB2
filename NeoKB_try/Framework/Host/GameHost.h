#ifndef GAME_HOST_H
#define GAME_HOST_H



#include <vector>

using namespace std;


namespace Framework {
namespace Host {

	/// <summary>
	/// �C���B�檺�D��
	/// TODO: �~��IIPCHOST
	/// </summary>
	class GameHost {
	
	public:
		
		GameHost(String name = "");


		int Run(Game game);

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
		/// ��input���main interface��
		/// </summary>
		GameThread InputThread;

		double MaximunInputHz;

		int InputInitialize();

		int InputFrame();

	private :

		static GameHost instance;

		FrameworkConfigManager frameworkConfigManager;

		int setupConfig();

		int bootstrapSceneGraph(Game game);

		MainInterface mainInterface;

		ChildAddable dependencies;

		/// <summary>
		/// input manager�|����root�A���h���J�ǵ��U��������
		/// </summary>
		ChildAddable root;

		vector<InputHandler*>* availableInputHandler;

	};

}}


#endif