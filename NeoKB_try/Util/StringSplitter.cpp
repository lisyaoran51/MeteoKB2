#include "StringSplitter.h"


using namespace Util;


vector<string> StringSplitter::Split(string s, string splitter)
{
	string::size_type pos;
	vector<string> result;
	s += splitter;//扩展字符串以方便操作
	int size = s.size();

	for (int i = 0; i < size; i++)
	{
		pos = s.find(splitter, i);
		if (pos < size)
		{
			string split = s.substr(i, pos - i);
			result.push_back(split);
			i = pos + splitter.size() - 1;
		}
	}
	return result;
}

vector<string> StringSplitter::Split(string s, string splitter1, string splitter2)
{
	return vector<string>();
}

string StringSplitter::Combine(vector<string> element, string delimiter)
{
	string result = "";
	for (int i = 0; i < element.size(); i++) {
		if (i != 0)
			result += delimiter;
		result += element[i];
	}
		
	return result;
}
