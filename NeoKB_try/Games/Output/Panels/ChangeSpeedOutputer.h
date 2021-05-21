#ifndef CHANGE_SPEED_OUTPUTER_H
#define CHANGE_SPEED_OUTPUTER_H


#include "ChangeSpeedPanelMessage.h"
#include "../../../Framework/Output/Panels/PanelOutputer.h"
#include "../../../Framework/IO/Panel.h"


using namespace Framework::Output::Panels;
using namespace Framework::IO;


namespace Games {
namespace Output{
namespace Panels{

	class ChangeSpeedOutputer : public TPanelOutputer<ChangeSpeedPanelMessage> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface);

	protected:

		virtual int pushMessage(ChangeSpeedPanelMessage* outputMessag);

		Panel* panel = nullptr;

	};

}}}








#endif
