#ifndef OUTPUT_MESSAGE_H
#define OUTPUT_MESSAGE_H


#include <string>

using namespace std;


namespace Framework {
namespace Output{

	class OutputMessage {


	public:

		virtual string ToString() = 0;

		virtual OutputMessage* Clone() = 0;

	};

}}







#endif