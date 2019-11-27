#ifndef CHILD_ADDABLE_H
#define CHILD_ADDABLE_H

#include "Loadable.h"
#include <vector>
#include <mutex>

using namespace std;

namespace Framework {
namespace Allocation {
namespace Hierachal{


	mutex ChildMutex;

	/// <summary>
	/// able to add to a parent object
	/// </summary>
	class ChildAddable: public Loadable {

		/// <summary>
		/// this vector is applied every time you call add_child()
		/// </summary>
		vector<MTO_FUNC_POINTER> onAdd;

		vector<ChildAddable*> childs;

		bool isAlive = false;


	public:

		ChildAddable();

		virtual ~ChildAddable() = default;

		/// <summary>
		/// when add child, automatically call every privateLoad() to load in parent's configuration
		/// </summary>
		virtual int AddChild(ChildAddable* child);

		virtual int DeleteChild(ChildAddable* child);

		int RegisterOnAdd(MTO_FUNC_POINTER func);

		/// <summary>
		/// 使用時記得要用lock child mutex
		/// </summary>
		vector<ChildAddable*>* GetChilds();

		bool GetIsAlive();

		/// <summary>
		/// 讓他死掉
		/// TODO: 這個應該要擺在OnDelete裡面去更改他的alive，不能這樣映改，然後再delete child時觸發
		/// </summary>
		int SetIsAlive(bool value);

		int _DebugPrintTree(string space);
	};




}}}




#endif