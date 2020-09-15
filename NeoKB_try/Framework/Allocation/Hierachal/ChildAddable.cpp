#include "ChildAddable.h"

#include <algorithm>


using namespace Framework::Allocation::Hierachal;
using namespace std;

mutex Framework::Allocation::Hierachal::ChildMutex;
/// <summary>
/// 這些是用來再update thread更改child時，禁止draw thread和input thread使用child tree，避免錯誤
/// </summary>
mutex Framework::Allocation::Hierachal::TreeMutex1;
/// <summary>
/// 這些是用來再update thread更改child時，禁止draw thread和input thread使用child tree，避免錯誤
/// </summary>
mutex Framework::Allocation::Hierachal::TreeMutex2;
/// <summary>
/// 這些是用來再update thread更改child時，禁止draw thread和input thread使用child tree，避免錯誤
/// </summary>
mutex Framework::Allocation::Hierachal::TreeMutex3;

ChildAddable::ChildAddable(): Loadable(), RegisterType("ChildAddable")
{
}

ChildAddable::~ChildAddable()
{
	/*
	
	*/
	//unique_lock<mutex> uLock(ChildMutex);

	for (int i = 0; i < childs.size(); i++) {

		childs[i]->_DebugPrintTree(" " + string("-"));

	}

	for (int i = 0; i < childs.size(); i++) {
		LOG(LogLevel::Debug) << "ChildAddable::~ChildAddable() : delete child [" << childs.at(i)->GetTypeName() << "].";

		

		delete childs.at(i);
	}

	childs.clear();
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
			unique_lock<mutex> uLock2(TreeMutex1);
			unique_lock<mutex> uLock3(TreeMutex2);
			unique_lock<mutex> uLock4(TreeMutex3);
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

	unique_lock<mutex> uLock2(TreeMutex1);
	unique_lock<mutex> uLock3(TreeMutex2);
	unique_lock<mutex> uLock4(TreeMutex3);
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

vector<ChildAddable*>* ChildAddable::DeleteChild(string name)
{
	vector<ChildAddable*>* deletedChilds = new vector<ChildAddable*>();

	for (int i = 0; i < childs.size(); i++) {
		if (childs[i]->GetTypeName() == name) {
			deletedChilds->push_back(childs[i]);
			childs.erase(childs.begin() + i);
			i--;
		}
	}



	return deletedChilds;
}

vector<ChildAddable*>* ChildAddable::DeleteChilds()
{
	vector<ChildAddable*>* deletedChilds = new vector<ChildAddable*>();

	unique_lock<mutex> uLock2(TreeMutex1);
	unique_lock<mutex> uLock3(TreeMutex2);
	unique_lock<mutex> uLock4(TreeMutex3);
	unique_lock<mutex> uLock(ChildMutex);

	for (int i = 0; i < childs.size(); i++) {
		deletedChilds->push_back(childs[i]);
	}

	childs.clear();

	return deletedChilds;
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
