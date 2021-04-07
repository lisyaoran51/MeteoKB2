#include "SustainPedalLightRingPanelMessage.h"

#include <math.h>
#include <sstream>
#include "../../../Util/Log.h"



using namespace Meteor::Output::Panels;
using namespace std;
using namespace std::literals::string_literals;
using namespace Util;


string SustainPedalLightRingPanelMessage::roundAndToString(float number, int digit)
{

	ostringstream ss;
	ss << number;
	string s(ss.str());

	return s;


	int dotPos = 999;
	string toString = to_string(number);
	for (int i = toString.length() - 1; i >= 0; i--) {
		if (toString.at(i) == '.')
			dotPos = i + 1;
	}
	if (dotPos == 999)
		return toString;

	toString = toString.substr(0, (dotPos + digit < toString.length() ? dotPos + digit : toString.length()));

	return toString;
}

SustainPedalLightRingPanelMessage::SustainPedalLightRingPanelMessage(float tLifeTime, float pDownLifeTime, float rLifeTime):
	targetLifeTime(tLifeTime), pedalDownLifeTime(pDownLifeTime), ringLifeTime(rLifeTime)
{

}

string SustainPedalLightRingPanelMessage::ToString()
{
	// TODO: §ï¦W¦¨Light Ring
	LOG(LogLevel::Depricated) << "SustainPedalLightRingPanelMessage::ToString() : pass [" << ("SP,"s + roundAndToString(targetLifeTime, 2) + ","s + roundAndToString(pedalDownLifeTime, 2) + ","s + roundAndToString(ringLifeTime, 2)) << "] to board.";
	return "RV,"s + roundAndToString(pedalDownLifeTime, 2);
	return "SP,"s + roundAndToString(targetLifeTime, 2) + ","s + roundAndToString(pedalDownLifeTime, 2) + ","s + roundAndToString(ringLifeTime, 2);
}

OutputMessage * SustainPedalLightRingPanelMessage::Clone()
{
	return new SustainPedalLightRingPanelMessage(targetLifeTime, pedalDownLifeTime, ringLifeTime);
}
