#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H


#include <vector>
#include "InputState.h"
#include "KeyboardState.h"


using namespace std;


/* 參考下面
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
	/// 當做參數送給有註冊的delegate函式
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