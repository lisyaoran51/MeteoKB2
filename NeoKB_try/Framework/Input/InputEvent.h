#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


#include <vector>
#include "InputState.h"
#include "KeyboardState.h"


using namespace std;


/* �ѦҤU��
public class KeyboardKeyEventArgs : EventArgs
{
	public KeyboardKeyEventArgs();
	public KeyboardKeyEventArgs(KeyboardKeyEventArgs args);
	public Key Key{ get; }
		[CLSCompliant(false)]
	public uint ScanCode{ get; }
	public bool Alt{ get; }
	public bool Control{ get; }
	public bool Shift{ get; }
	public KeyModifiers Modifiers{ get; }
	public KeyboardState Keyboard{ get; }
	public bool IsRepeat{ get; }
}
*/



namespace Framework {
namespace Input {

	/// <summary>
	/// ���Ѽưe�������U��delegate�禡
	/// </summary>
	class InputEvent {

	public:

		InputEvent();

		InputState* Clone();

	protected:


	private:

		Key key;

	};


}}

#endif