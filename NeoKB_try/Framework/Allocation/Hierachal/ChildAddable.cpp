#include "ChildAddable.h"


using namespace Framework::Allocation::Hierachal;


ChildAddable::ChildAddable(): Loadable(), RegisterType("ChildAddable")
{
}

int ChildAddable::AddChild(ChildAddable * child)
{
	if (child->SetParent(this) == DONE) {
		child->Load();

		for (int i = 0; i < onAdd.size(); i++)
			onAdd[i]();

		vector<ChildAddable*>::iterator it = find(childs.begin(), childs.end(), child);

		if (it != childs.end()) {
			// TODO: throw error
		}
		else
			childs.push_back(child);

		return DONE;
	}
	

	// TODO: 例外處理
	return -1;
}

int ChildAddable::DeleteChild(ChildAddable * child)
{
	vector<ChildAddable*>::iterator it = find(childs.begin(), childs.end(), child);

	if (it != childs.end()) 
		childs.erase(it);
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
