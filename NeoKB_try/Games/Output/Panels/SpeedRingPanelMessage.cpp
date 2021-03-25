#include "SpeedRingPanelMessage.h"


using namespace Games::Output::Panels;



SpeedRingPanelMessage::SpeedRingPanelMessage(vector<bool> l)
{
}

SpeedRingPanelMessage::SpeedRingPanelMessage(int p)
{
	position = p;
}

string SpeedRingPanelMessage::ToString()
{
	if (position != -5)
		return string("SK,") + to_string(position) + string("\0");
	else
		return string("\0");
}
