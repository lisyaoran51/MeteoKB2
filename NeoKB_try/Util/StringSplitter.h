#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <string>
#include <vector>



namespace Util {

	/// <summary>
	/// 可以把字串分歌，還會自動削去前後空白
	/// </summary>
	class StringSplitter {

	public:

		/// <summary>
		/// 如果最後一個字也是splitter的話就會出錯，之後要改一下
		/// </summary>
		static std::vector<std::string> Split(std::string s, std::string splitter);

		/// <summary>
		/// 這個懶得寫，先不要用
		/// </summary>
		static std::vector<std::string> Split(std::string s, std::string splitter1, std::string splitter2);

		static std::string Combine(std::vector<std::string> element, std::string delimiter);

	};


}



#endif