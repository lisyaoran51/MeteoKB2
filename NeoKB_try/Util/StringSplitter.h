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

		/// <summary>
		/// �p�G�̫�@�Ӧr�]�Osplitter���ܴN�|�X���A����n��@�U
		/// </summary>
		static vector<string> Split(string s, string splitter);

		/// <summary>
		/// �o���i�o�g�A�����n��
		/// </summary>
		static vector<string> Split(string s, string splitter1, string splitter2);

		static string Combine(vector<string> element, string delimiter);

	};


}



#endif