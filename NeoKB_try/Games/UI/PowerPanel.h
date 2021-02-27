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
		/// �q���C���������n���j��5��A���M�ȶ}����@�b�N�Q����
		/// </summary>
		double lastSwitchTime = 4;	//���w�]�}��4���~������

		bool isPowerOn = true;

		virtual int onButtonUp(InputState* inputState, InputKey button);
		


	};

}}





#endif