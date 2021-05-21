#include "ChangeSpeedPanelMessage.h"

#include <math.h>
#include <sstream>
#include "../../../Util/Log.h"



using namespace Games::Output::Panels;
using namespace std;
using namespace std::literals::string_literals;
using namespace Util;


string ChangeSpeedPanelMessage::roundAndToString(float number, int digit)
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

ChangeSpeedPanelMessage::ChangeSpeedPanelMessage(float s)
{
	speed = s;
}

string ChangeSpeedPanelMessage::ToString()
{
	return "BT,"s + roundAndToString(speed, 2);
}

OutputMessage * ChangeSpeedPanelMessage::Clone()
{
	return new ChangeSpeedPanelMessage(speed);
}