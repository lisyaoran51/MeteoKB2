#ifndef FILE_CONTENT_GETTER_H
#define FILE_CONTENT_GETTER_H


#include <string>


using namespace std;


namespace Util {


	class FileContentGetter {

	public:

		static string GetContentFromFile(char const* fname);

		static string GetFileContentVariable(char const* file, char const* field);
		

	};


}




#endif