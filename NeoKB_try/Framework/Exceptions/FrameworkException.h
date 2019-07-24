#ifndef FRAMEWORK_EXCEPTION_H
#define FRAMEWORK_EXCEPTION_H


#include <exception>

using namespace std;


namespace Framework {
namespace Exceptions {

	struct FrameworkError : public runtime_error {

		FrameworkError(char* message) : runtime_error(message) {}

		const char * what() const throw () {
			return runtime_error::what();
		}
	};

}
}







#endif