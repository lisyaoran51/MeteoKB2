#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H


#include <vector>

using namespace std;


namespace Framework {
namespace Input {

	/// <summary>
	/// 抓取input，並且發送到所有子物件
	/// </summary>
	class InputManager : ChildAddable {

	public:

	protected:

		GameHost Host;

		int HandleNewState(InputState* state);

		vector<InputState*>* GetPendingState();

	private:

		/// <summary>
		/// 擺入gamehost
		/// </summary>
		int load();

		int Update();


		vector<InputState*>* createDistinctInputStates(vector<InputState*>* states);

		virtual int TransformState(InputState* inputState);

		int updateInputQueue(InputState* inputState);

		int updateKeyboardEvents(InputState* inputState);

		int updatePanelEvents(InputState* inputState);

		int handleKeyDown(InputState* state, Key key);

		int handleKeyUp(InputState* state, Key key);

	};


}}

#endif