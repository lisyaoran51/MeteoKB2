#ifndef PANEL_EVENT_H
#define PANEL_EVENT_H

#include "IoEvent.h"
#include "../../../../Framework/Input/KeyBindings/HasInputKey.h"


using namespace Framework::Input::KeyBindings;


namespace Games {
namespace Schedulers {
namespace Events {
namespace IoEvents {

	class PanelEvent : public IoEvent, public HasInputKey {

	public:

		PanelEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l);
		PanelEvent(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l);

	protected:


	};



}}}}








#endif
