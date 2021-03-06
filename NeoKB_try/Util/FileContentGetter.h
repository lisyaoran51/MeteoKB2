#ifndef FILE_CONTENT_GETTER_H
#define FILE_CONTENT_GETTER_H


#include <string>




namespace Util {


	class FileContentGetter {

	public:

		static std::string GetContentFromFile(char const* fname);

		static std::string GetFileContentVariable(char const* file, char const* field);
		

	};


}




#endif