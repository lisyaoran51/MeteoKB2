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
		/// �ϥήɰO�o�n��lock child mutex
		/// </summary>
		vector<ChildAddable*>* GetChilds();

		bool GetIsAlive();

		/// <summary>
		/// ���L����
		/// TODO: �o�����ӭn�\�bOnDelete�̭��h���L��alive�A����o�ˬM��A�M��Adelete child��Ĳ�o
		/// </summary>
		int SetIsAlive(bool value);

		int _DebugPrintTree(string space);
	};




}}}




#endif