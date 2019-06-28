#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>
#include "Updatable.h"
#include "../../Input/InputState.h"

using namespace std;
using namespace Framework::Input;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Triggerable : public Updatable {

	public:

		Triggerable();

		virtual int TriggerOnKeyDown(InputState* inputState);

	protected:

		virtual int onKeyDown(InputState* inputState);

	};

}}}







#endif