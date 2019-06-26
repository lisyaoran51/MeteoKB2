#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H



#include <vector>

using namespace std;


namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. �C��update���׳Qparent�I�s�A�̫��update thread�ӥsroot
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