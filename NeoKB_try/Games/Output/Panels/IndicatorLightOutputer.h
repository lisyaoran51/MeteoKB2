#ifndef INDICATOR_LIGHT_OUTPUTER_H
#define INDICATOR_LIGHT_OUTPUTER_H


#include "IndicatorLightPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Games {
namespace Output{
namespace Panels{

	class IndicatorLightOutputer : public TPanelOutputer<IndicatorLightPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);


	protected:

		virtual int pushMessage(IndicatorLightPanelMessage* outputMessage);

		Panel* panel = nullptr;

	};

}}}








#endif
