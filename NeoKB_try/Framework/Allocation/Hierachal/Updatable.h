#ifndef UPDATABLE_H
#define UPDATABLE_H


#include <vector>

using namespace std;


namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. �C��update���׳Qparent�I�s�A�̫��update thread�ӥsroot
	/// </summary>
	class Updatable : public ChildAddable {

		Clock* clock;

	public:

		Updatable();

		int SetClock();

		Clock* GetClock();

		/// <summary>
		/// �o��|override������set parent�A�M���clock�]��parent��clock
		/// </summary>
		int SetParent(HasParent* p);

	protected:

		virtual int update();

		/// <summary>
		/// �o��|����s���ۤv�A�M��A�I�schild��updateSubTree�C�I�s�e�n���T�wchild�O���Oupdatable
		/// </summary>
		int updateSubTree();

	};

}}}

#endif