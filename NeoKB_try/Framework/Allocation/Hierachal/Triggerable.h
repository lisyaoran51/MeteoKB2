#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>

using namespace std;


namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Triggerable : public Updatable {

		Clock* clock;

	public:

		int TriggerOnKeyDown(InputState* inputState);

		

	protected:

		int onKeyDown(InputState* inputState);

	};

}}}







#endif