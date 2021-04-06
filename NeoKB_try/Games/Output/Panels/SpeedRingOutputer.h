#ifndef SPEED_RING_OUTPUTER_H
#define SPEED_RING_OUTPUTER_H


#include "SpeedRingPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Games {
namespace Output{
namespace Panels{

	class SpeedRingOutputer : public TPanelOutputer<SpeedRingPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);


	protected:

		virtual int pushMessage(SpeedRingPanelMessage* outputMessage);

		Panel* panel = nullptr;

	};

}}}








#endif
