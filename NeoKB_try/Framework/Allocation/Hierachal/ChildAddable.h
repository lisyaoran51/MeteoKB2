#ifndef CHILD_ADDABLE_H
#define CHILD_ADDABLE_H

#include "Loadable.h"
#include <vector>

using namespace std;

namespace Framework {
namespace Allocation {
namespace Hierachal{

	/// <summary>
	/// able to add to a parent object
	/// </summary>
	class ChildAddable: public Loadable {

		/// <summary>
		/// this vector is applied every time you call add_child()
		/// </summary>
		vector<MTO_FUNC_POINTER> onAdd;

		vector<ChildAddable*> childs;

	public:

		ChildAddable();

		virtual ~ChildAddable() = default;

		/// <summary>
		/// when add child, automatically call every privateLoad() to load in parent's configuration
		/// </summary>
		int AddChild(ChildAddable* child);

		int DeleteChild(ChildAddable* child);

		int RegisterOnAdd(MTO_FUNC_POINTER func);

		vector<ChildAddable*>* GetChilds();



	};




}}}




#endif