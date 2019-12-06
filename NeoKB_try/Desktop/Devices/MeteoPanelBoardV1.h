#ifndef METEO_PANEL_BOARD_V1_H
#define METEO_PANEL_BOARD_V1_H


#include <vector>
#include "../../Framework/Input/InputState.h"
#include <mutex>
#include "Pi2c.h"


using namespace Framework::Input;
using namespace std;


/*
 * i2c�g�k:
 * https://github.com/JohnnySheppard/Pi2c
 * ���Ȯɸ�arduino�s�u
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

	protected:

		int work();

		int readPanel();

		int writePanel();

		int pushKeyboardState(InputKey key, int value);

		int pushPanelState(InputKey key, int value);
	};


}}




#endif