#include "IndicatorLightPanelMessage.h"


using namespace Games::Output::Panels;


IndicatorLightPanelMessage::IndicatorLightPanelMessage(int p, bool l)
{
	position = p;
	isLightUp = l;
}

IndicatorLightPanelMessage::IndicatorLightPanelMessage(IndicatorLight iLight, bool l)
{
	switch (iLight) {
	case IndicatorLight::Power:
		position = 0;
		break;
	case IndicatorLight::Sensitivity:
		position = 1;
		break;
	case IndicatorLight::Sustain:
		position = 2;
		break;
	case IndicatorLight::LowerOctave:
		position = 3;
		break;
	case IndicatorLight::RaiseOctave:
		position = 14;
		break;
	}
	isLightUp = l;
}

string IndicatorLightPanelMessage::ToString()
{
	if(isLightUp)
		return string("IR,") + to_string(position) + string(",1\0");
	else
		return string("IR,") + to_string(position) + string(",0\0");
}
