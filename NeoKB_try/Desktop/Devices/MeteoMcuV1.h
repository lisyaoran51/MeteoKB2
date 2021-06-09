#ifndef METEO_MCU_V1_H
#define METEO_MCU_V1_H


#include <vector>
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include "pi2c.h"
#include "../../Framework/Threading/SimpleThread.h"


using namespace Framework::Input;
using namespace std;
using namespace Framework::Threading;


/*
 * i2c寫法:
 * https://github.com/JohnnySheppard/Pi2c
 * 先暫時跟arduino連線
 */



namespace Desktop {
namespace Devices {

	class MeteoMcuV1 : public SimpleThread {

		Pi2c* i2cInterface;

		mutable mutex panelStateMutex, keyboardStateMutex, i2cMessageMutex;

		/* read */
		InputState* panelState;

		InputState* keyboardState;

		bool exitRequested = false;

		vector<string> split(string s, string pattern);

		string trim(string s);

		/* write */
		vector<string> i2cMessages;

		bool checkI2cMessageValid(InputKey iKey, int v);

	public:

		MeteoMcuV1(int address);

		/* read */
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

		/* write */
		/// <summary>
		/// 把要傳過去panel的i2c訊息轉成文字
		/// </summary>
		int PushI2cMessage(string iMessage);

	protected:

		int work();

		int readPanel();

		int writePanel();

		int pushKeyboardState(InputKey key, int value);

		int pushPanelState(InputKey key, int value);
	};


}}




#endif