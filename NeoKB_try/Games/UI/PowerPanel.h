#ifndef POWER_PANEL_H
#define POWER_PANEL_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../../Framework/Output/OutputManager.h"
#include "../Output/Bluetooths/MeteoContextBluetoothMessage.h"


using namespace Framework::Output;
using namespace Games::Output::Bluetooths;


namespace Games {
namespace UI{

	class PowerPanel : public Container {

		int load();

		int load(OutputManager* o);

	public:

		PowerPanel();

		function<int(void)> PowerOffRequest;

		function<int(void)> PowerOnRequest;

	protected:

		OutputManager* outputManager = nullptr;

		/// <summary>
		/// 電源每次切換都要間隔約5秒，不然怕開機到一半就被關機
		/// </summary>
		double lastSwitchTime = 4;	//先預設開機4秒後才能關機

		bool isPowerOn = true;

		virtual int onButtonUp(InputState* inputState, InputKey button);
		


	};

}}





#endif