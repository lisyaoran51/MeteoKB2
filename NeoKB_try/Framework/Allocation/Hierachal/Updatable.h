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
		FrameBasedClock* customClock = nullptr;

	public:

		Updatable();

		int SetClock(FrameBasedClock* c);

		FrameBasedClock* GetClock();

		FrameBasedClock* GetCustomClock();

		/// <summary>
		/// �o��|override������set parent�A�M���clock�]��parent��clock
		/// </summary>
		virtual int SetParent(HasParent* p);

		/// <summary>
		/// �o��|����s���ۤv�A�M��A�I�schild��updateSubTree�C�I�s�e�n���T�wchild�O���Oupdatable
		/// </summary>
		virtual bool UpdateSubTree();

		/// <summary>
		/// ��component��clock�Mcache�]�n�A�M��async
		/// </summary>
		virtual int LoadAsync(FrameBasedClock* c, DependencyContainer* dContainer);

	protected:

		virtual int update();
	};

}}}

#endif