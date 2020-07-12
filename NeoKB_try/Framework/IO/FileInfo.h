#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <string>

using namespace std;


namespace Framework {
namespace IO{

	struct FileInfo {

		int Id;

		string Path;

		int ReferenceCount;

	};

}}



#endif