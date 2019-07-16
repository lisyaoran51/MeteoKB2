#include "Updatable.h"

using namespace Framework::Allocation::Hierachal;


Updatable::Updatable() : ChildAddable(), RegisterType("Updatable")
{
}

int Updatable::SetClock(FrameBasedClock* c)
{
	clock = c;
	return DONE;
}

FrameBasedClock * Updatable::GetClock()
{
	return clock;
}

int Updatable::SetParent(HasParent * p)
{
	Updatable* parent = Cast<Updatable>(p);
	if (p != nullptr) {
		clock = parent->GetClock();
	}
	else {
		throw invalid_argument("Updatable::SetParent: parent has to be updatable.");
	}
	
	return Loadable::SetParent(p);
}

int Updatable::UpdateSubTree()
{
	update();

	vector<ChildAddable*>* childs = GetChilds();
	for (vector<ChildAddable*>::iterator iter = childs->begin(); iter != childs->end(); iter++) {
		Updatable* child = Cast<Updatable>(*iter);
		if(child != nullptr)
			Cast<Updatable>(*iter)->UpdateSubTree();
	}
	return 0;
}

int Updatable::update()
{
	return 0;
}
