#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <string>
#include <vector>

using namespace std;


namespace Util {

	/// <summary>
	/// �i�H��r����q�A�ٷ|�۰ʫd�h�e��ť�
	/// </summary>
	class StringSplitter {

	public:

		static vector<string> Split(string s, char splitter);
		static vector<string> Split(string s, char splitter1, char splitter2);

	};


}



#endif