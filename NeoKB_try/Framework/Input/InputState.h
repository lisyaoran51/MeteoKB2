#ifndef INPUT_STATE_H
#define INPUT_STATE_H


#include <vector>

using namespace std;


namespace Framework {
namespace Input {

	/// <summary>
	/// ���Ѽưe�������U��delegate�禡
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