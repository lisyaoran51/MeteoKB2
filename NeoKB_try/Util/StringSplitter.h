#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <string>
#include <vector>

using namespace std;


namespace Util {

	/// <summary>
	/// 可以把字串分歌，還會自動削去前後空白
	/// </summary>
	class StringSplitter {

	public:

		static vector<string> Split(string s, char splitter);
		static vector<string> Split(string s, char splitter1, char splitter2);

	};


}



#endif