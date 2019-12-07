#ifndef METEO_PANEL_BOARD_V1_H
#define METEO_PANEL_BOARD_V1_H


#include <vector>
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include "pi2c.h"


using namespace Framework::Input;
using namespace std;


/*
 * i2c寫法:
 * https://github.com/JohnnySheppard/Pi2c
 * 先暫時跟arduino連線
 */



namespace Desktop {
namespace Devices {

	class MeteoPanelBoardV1 {

		Pi2c* i2cInterface;

		mutable mutex panelStateMutex, keyboardStateMutex;

		InputState* panelState;

		InputState* keyboardState;

		bool exitRequested = false;

		vector<string> split(string s, string pattern);

		string trim(string s);

	public:

		MeteoPanelBoardV1(int address);

		InputState* GetPanelState();

		InputState* GetKeyboardState();

		/// <summary>
		/// 亮燈往左或往右
		/// </summary>
		int SetSpeedKnobLight(int length);

		/// <summary>
		/// 下一圈繞多久
		/// </summary>
		int SetSectionKnobLight(double roundTime);

		/// <summary>
		/// 點亮哪個按鍵的燈
		/// </summary>
		int SwitchLight(int button, bool isTurnOn);

	protected:

		int work();

		int readPanel();

		int writePanel();

		int pushKeyboardState(InputKey key, int value);

		int pushPanelState(InputKey key, int value);
	};


}}




#endif