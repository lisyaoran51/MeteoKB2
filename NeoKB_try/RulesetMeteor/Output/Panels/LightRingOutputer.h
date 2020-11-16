#ifndef LIGHT_RING_OUTPUTER_H
#define LIGHT_RING_OUTPUTER_H


#include "LightRingPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Meteor {
namespace Output{
namespace Panels{

	class LightRingOutputer : public TPanelOutputer<LightRingPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);


	protected:

		virtual int pushMessage(LightRingPanelMessage* outputMessag);

		Panel* panel = nullptr;

	};

}}}








#endif
