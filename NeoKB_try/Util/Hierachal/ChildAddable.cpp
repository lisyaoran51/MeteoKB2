#include "ChildAddable.h"

using namespace Util::Hierachal;


ChildAddable::ChildAddable(): Loadable(), RegisterType("ChildAddable")
{
}

int ChildAddable::AddChild(ChildAddable * child)
{
	if (child->SetParent(this) == 0) {
		child->Load();

		for (int i = 0; i < onAdd.size(); i++)
			onAdd[i]();

		return 0;
	}
	

	// TODO: ¨Ò¥~³B²z
	return -1;
}


int ChildAddable::RegisterOnAdd(MTO_FUNC_POINTER func)
{
	onAdd.push_back(func);
	return 0;
}
