#ifndef INPUT_STATE_H
#define INPUT_STATE_H


#include <vector>

using namespace std;


namespace Framework {
namespace Input {

	/// <summary>
	/// 當做參數送給有註冊的delegate函式
	/// </summary>
	class InputState {

	public:

		InputState* Clone();

	protected:


	private:

		KeyboardState* keyboardState;

		PanelState* panelState;

	};


}}

#endif