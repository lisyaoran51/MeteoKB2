#ifndef UPDATABLE_H
#define UPDATABLE_H


#include <vector>

using namespace std;


namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// every class inherit form this has a function to update. 每個update都匯被parent呼叫，最後由update thread來叫root
	/// </summary>
	class Updatable : public ChildAddable {

		Clock* clock;

	public:

		Updatable();

		int SetClock();

		Clock* GetClock();

		/// <summary>
		/// 這邊會override元本的set parent，然後把clock設成parent的clock
		/// </summary>
		int SetParent(HasParent* p);

	protected:

		virtual int update();

		/// <summary>
		/// 這邊會先更新完自己，然後再呼叫child的updateSubTree。呼叫前要先確定child是不是updatable
		/// </summary>
		int updateSubTree();

	};

}}}

#endif