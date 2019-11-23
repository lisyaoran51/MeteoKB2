#include "Updatable.h"

using namespace Framework::Allocation::Hierachal;




Updatable::Updatable() : ChildAddable(), RegisterType("Updatable")
{
}

int Updatable::SetClock(FrameBasedClock* c)
{
	customClock = c;
	return 0;
}

FrameBasedClock * Updatable::GetClock()
{
	return clock;
}

FrameBasedClock * Updatable::GetCustomClock()
{
	return customClock;
}

int Updatable::SetParent(HasParent * p)
{
	Updatable* parent = Cast<Updatable>(p);
	if (p != nullptr) {
		clock = parent->GetCustomClock() != nullptr ? parent->GetCustomClock() : parent->GetClock();
	}
	else {
		throw invalid_argument("Updatable::SetParent: parent has to be updatable.");
	}
	
	return Loadable::SetParent(p);
}

bool Updatable::UpdateSubTree()
{

	if (GetLoadState() < LoadState::Ready)
		return false;

	if (GetLoadState() == LoadState::Ready)
		LoadComplete();

	if (GetParent() != nullptr && customClock != nullptr) {
		LOG(LogLevel::Finest) << "Updatable::UpdateSubTree() : process custom clock [" << customClock << "].";
		customClock->ProcessFrame();
	}

	update();

	vector<ChildAddable*>* childs = GetChilds();
	for (vector<ChildAddable*>::iterator iter = childs->begin(); iter != childs->end(); iter++) {
		Updatable* child = Cast<Updatable>(*iter);
		if(child != nullptr)
			Cast<Updatable>(*iter)->UpdateSubTree();
	}

	

	return true;
}

int Updatable::LoadAsync(FrameBasedClock * c, DependencyContainer * dContainer)
{
	LOG(LogLevel::Fine) << "int Updatable::LoadAsync() : A [" << GetTypeName() << "] object is asyncing with clock and cache.";
	clock = c;
	return Loadable::LoadAsync(dContainer);
}

int Updatable::update()
{
	return 0;
}



