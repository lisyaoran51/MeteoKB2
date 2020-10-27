#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <vector>
#include "../../Host/GameHost.h"

using namespace std;
using namespace Framework::Host;

namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// 負責去host抓輸入
	/// </summary>
	class InputHandler {

	public:

		virtual int Initialize(GameHost* host) = 0;

		virtual vector<InputState*>* GetPendingStates();

		virtual int HandleState(InputState* inputEvent) = 0;

	protected:
		
		/// <summary>
		/// TODO: 應該要用concurrency
		/// </summary>
		vector<InputState*> pendingStates;

	private:

		

	};


}}}

#endif