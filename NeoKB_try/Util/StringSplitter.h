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

		/// <summary>
		/// 如果最後一個字也是splitter的話就會出錯，之後要改一下
		/// </summary>
		static vector<string> Split(string s, string splitter);

		/// <summary>
		/// 這個懶得寫，先不要用
		/// </summary>
		static vector<string> Split(string s, string splitter1, string splitter2);

		static string Combine(vector<string> element, string delimiter);

	};


}



#endif