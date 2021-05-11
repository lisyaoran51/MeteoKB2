#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <vector>
#include "../../Hosts/Host.h"

using namespace std;
using namespace Framework::Hosts;

namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// �t�d�hhost���J
	/// </summary>
	class InputHandler {

	public:

		virtual int Initialize(Host* host) = 0;

		virtual vector<InputState*>* GetPendingStates();

		virtual int HandleState(InputState* inputEvent) = 0;

	protected:
		
		/// <summary>
		/// TODO: ���ӭn��concurrency
		/// </summary>
		vector<InputState*> pendingStates;

	private:

		

	};


}}}

#endif