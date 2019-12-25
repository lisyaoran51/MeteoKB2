#include "PanelEvent.h"


using namespace Games::Schedulers::Events::IoEvents;




PanelEvent::PanelEvent(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l): IoEvent(s, l), HasInputKey(iKey, 0)
{
}

PanelEvent::PanelEvent(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l) : IoEvent(s, l), HasInputKey(iKey, iValue)
{
}
