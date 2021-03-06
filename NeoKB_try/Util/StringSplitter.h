#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <string>
#include <vector>



namespace Util {

	/// <summary>
	/// �i�H��r����q�A�ٷ|�۰ʫd�h�e��ť�
	/// </summary>
	class StringSplitter {

	public:

		/// <summary>
		/// �p�G�̫�@�Ӧr�]�Osplitter���ܴN�|�X���A����n��@�U
		/// </summary>
		static std::vector<std::string> Split(std::string s, std::string splitter);

		/// <summary>
		/// �o���i�o�g�A�����n��
		/// </summary>
		static std::vector<std::string> Split(std::string s, std::string splitter1, std::string splitter2);

		static std::string Combine(std::vector<std::string> element, std::string delimiter);

	};


}



#endif