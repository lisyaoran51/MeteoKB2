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
 * i2c�g�k:
 * https://github.com/JohnnySheppard/Pi2c
 * ���Ȯɸ�arduino�s�u
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
		/// �G�O�����Ω��k
		/// </summary>
		int SetSpeedKnobLight(int length);

		/// <summary>
		/// �U�@��¶�h�[
		/// </summary>
		int SetSectionKnobLight(double roundTime);

		/// <summary>
		/// �I�G���ӫ��䪺�O
		/// </summary>
		int SwitchLight(int button, bool isTurnOn);

		/* write */
		/// <summary>
		/// ��n�ǹL�hpanel��i2c�T���ন��r
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