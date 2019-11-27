#include "ChildAddable.h"

#include <algorithm>


using namespace Framework::Allocation::Hierachal;
using namespace std;

mutex ChildMutex;

ChildAddable::ChildAddable(): Loadable(), RegisterType("ChildAddable")
{
}

int ChildAddable::AddChild(ChildAddable * child)
{
	if (child->SetParent(this) == 0) {
		child->Load();

		for (int i = 0; i < onAdd.size(); i++)
			onAdd[i]();

		vector<ChildAddable*>::iterator it = find(childs.begin(), childs.end(), child);

		if (it != childs.end()) {
			// TODO: throw error
		}
		else {
			unique_lock<mutex> uLock(ChildMutex);
			childs.push_back(child);
		}

		isAlive = true;

		return 0;
	}
	

	// TODO: 例外處理
	return -1;
}

int ChildAddable::DeleteChild(ChildAddable * child)
{
	unique_lock<mutex> uLock(ChildMutex);
	vector<ChildAddable*>::iterator it = find(childs.begin(), childs.end(), child);

	if (it != childs.end()) {
		(*it)->SetIsAlive(false);
		childs.erase(it);
	}
	else {
		// TODO: 例外處理
	}
	return 0;
}


int ChildAddable::RegisterOnAdd(MTO_FUNC_POINTER func)
{
	onAdd.push_back(func);
	return 0;
}

vector<ChildAddable*>* ChildAddable::GetChilds()
{
	return &childs;
}

bool ChildAddable::GetIsAlive()
{
	return isAlive;
}

int ChildAddable::SetIsAlive(bool value)
{
	isAlive = value;
	return 0;
}

int ChildAddable::_DebugPrintTree(string space)
{
	LOG(LogLevel::Debug) << space << GetTypeName();

	for (int i = 0; i < childs.size(); i++) {

		childs[i]->_DebugPrintTree(space + string("-"));

	}


	return 0;
}
