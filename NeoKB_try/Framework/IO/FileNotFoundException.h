#ifndef FILE_NOT_FOUND_EXCEPTION_H
#define FILE_NOT_FOUND_EXCEPTION_H

#include <stdexcept>

using namespace std;


namespace Framework {
namespace IO{


	struct FileNotFoundException : public runtime_error {

		FileNotFoundException(char* message) : runtime_error(message) {}

		const char * what() const throw () {
			return runtime_error::what();
		}
	};

}}




#endif