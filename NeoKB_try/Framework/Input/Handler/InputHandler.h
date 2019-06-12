#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include <vector>

using namespace std;


namespace Framework {
namespace Input {
namespace Handler {

	/// <summary>
	/// �t�d�hhost���J
	/// </summary>
	class InputHandler {

	public:

		virtual int Initialize(GameHost host) = 0;

		virtual vector<InputState*>* GetPendingStates();

	protected:
		
		/// <summary>
		/// TODO: ���ӭn��concurrency
		/// </summary>
		vector<InputState*>* PendingStates;

	private:

		

	};


}}}

#endif