#include "SustainPedalLightRingPanelMessage.h"

#include <math.h>
#include <sstream>



using namespace Meteor::Output::Panels;
using namespace std;
using namespace std::literals::string_literals;


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

SustainPedalLightRingPanelMessage::SustainPedalLightRingPanelMessage(float tLifeTime, float rLifeTime, float nTargetStartTime):
	targetLifeTime(tLifeTime), ringLifeTime(rLifeTime), nextTargetStartTime(nTargetStartTime)
{

}

string SustainPedalLightRingPanelMessage::ToString()
{
	return "SP,"s + roundAndToString(targetLifeTime, 3) + ","s + roundAndToString(ringLifeTime, 3) + ","s + roundAndToString(nextTargetStartTime, 3);
}
