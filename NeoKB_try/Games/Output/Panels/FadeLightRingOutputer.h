#ifndef FADE_LIGHT_RING_OUTPUTER_H
#define FADE_LIGHT_RING_OUTPUTER_H


#include "FadeLightRingPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Games {
namespace Output{
namespace Panels{

	class FadeLightRingOutputer : public TPanelOutputer<FadeLightRingPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);

	protected:

		virtual int pushMessage(FadeLightRingPanelMessage* outputMessag);

		Panel* panel = nullptr;

	};

}}}








#endif
