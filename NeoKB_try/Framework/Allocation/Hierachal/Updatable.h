#ifndef UPDATABLE_H
#define UPDATABLE_H


#include <vector>
#include "ChildAddable.h"
#include "../../Timing/FrameBasedClock.h"

using namespace std;
using namespace Framework::Timing;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. �C��update���׳Qparent�I�s�A�̫��update thread�ӥsroot
	/// </summary>
	class Updatable : public ChildAddable {

		FrameBasedClock* clock;

	public:

		Updatable();

		int SetClock(FrameBasedClock* c);

		FrameBasedClock* GetClock();

		/// <summary>
		/// �o��|override������set parent�A�M���clock�]��parent��clock
		/// </summary>
		int SetParent(HasParent* p);

		/// <summary>
		/// �o��|����s���ۤv�A�M��A�I�schild��updateSubTree�C�I�s�e�n���T�wchild�O���Oupdatable
		/// </summary>
		int UpdateSubTree();

	protected:

		virtual int update();


	};

}}}

#endif