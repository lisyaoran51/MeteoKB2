#ifndef REVOLVE_LIGHT_RING_OUTPUTER_H
#define REVOLVE_LIGHT_RING_OUTPUTER_H


#include "RevolveLightRingPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Games {
namespace Output{
namespace Panels{

	class RevolveLightRingOutputer : public TPanelOutputer<RevolveLightRingPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);

	protected:

		virtual int pushMessage(RevolveLightRingPanelMessage* outputMessag);

		Panel* panel = nullptr;

	};

}}}








#endif
