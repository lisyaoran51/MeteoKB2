#ifndef SUSTAIN_PEDAL_LIGHT_RING_OUTPUTER_H
#define SUSTAIN_PEDAL_LIGHT_RING_OUTPUTER_H


#include "SustainPedalLightRingPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Meteor {
namespace Output{
namespace Panels{

	class SustainPedalLightRingOutputer : public TPanelOutputer<SustainPedalLightRingPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);


	protected:

		virtual int pushMessage(SustainPedalLightRingPanelMessage* outputMessag);

		Panel* panel = nullptr;

	};

}}}








#endif
