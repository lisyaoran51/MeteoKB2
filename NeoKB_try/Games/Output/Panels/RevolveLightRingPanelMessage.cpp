#include "RevolveLightRingPanelMessage.h"

#include <math.h>
#include <sstream>
#include "../../../Util/Log.h"



using namespace Games::Output::Panels;
using namespace std;
using namespace std::literals::string_literals;
using namespace Util;


string RevolveLightRingPanelMessage::roundAndToString(float number, int digit)
{

	ostringstream ss;
	ss << number;
	string s(ss.str());

	return s;

	// 以下為測試用
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

RevolveLightRingPanelMessage::RevolveLightRingPanelMessage(float lTime) :
	targetLifeTime(0), pedalDownLifeTime(lTime), ringLifeTime(lTime)
{
	lifeTime = lTime;
}

string RevolveLightRingPanelMessage::ToString()
{
	// TODO: 改名成Light Ring
	LOG(LogLevel::Depricated) << "SustainPedalLightRingPanelMessage::ToString() : pass [" << ("SP,"s + roundAndToString(targetLifeTime, 2) + ","s + roundAndToString(pedalDownLifeTime, 2) + ","s + roundAndToString(ringLifeTime, 2)) << "] to board.";
	//return "SP,"s + roundAndToString(targetLifeTime, 2) + ","s + roundAndToString(pedalDownLifeTime, 2) + ","s + roundAndToString(ringLifeTime, 2);

	// 之後再改成這樣
	return "RV,"s + roundAndToString(lifeTime, 2);
}

OutputMessage * RevolveLightRingPanelMessage::Clone()
{
	return new RevolveLightRingPanelMessage(lifeTime);
}
