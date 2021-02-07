#include "Updatable.h"

// debug¥Î
#include <chrono>
using namespace std::chrono;
// debug¥Î

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

	LOG(LogLevel::Finest) << "Updatable::UpdateSubTree() : update [" << GetTypeName() << "].";

	if (GetLoadState() < LoadState::Ready)
		return false;

	if (GetLoadState() == LoadState::Ready)
		LoadComplete();

	system_clock::time_point startTime = system_clock::now();


	try {
		if (GetParent() != nullptr && customClock != nullptr) {
			customClock->ProcessFrame();
		}

		LOG(LogLevel::Depricated) << "Updatable::UpdateSubTree() : before update [" << GetTypeName() << "].";
		update();
		isFirstUpdate = false;
		LOG(LogLevel::Depricated) << "Updatable::UpdateSubTree() : after update [" << GetTypeName() << "].";
	}
	catch (exception& e) {
		LOG(LogLevel::Error) << "Updatable::UpdateSubTree() : error [" << e.what() << "] by [" << GetTypeName() << "].";
		LOG(LogLevel::Error) << "Updatable::UpdateSubTree() : error [" << e.what() << "] by [" << GetTypeName() << "].";
		LOG(LogLevel::Error) << "Updatable::UpdateSubTree() : error [" << e.what() << "] by [" << GetTypeName() << "].";
		LOG(LogLevel::Error) << "Updatable::UpdateSubTree() : error [" << e.what() << "] by [" << GetTypeName() << "].";
		LOG(LogLevel::Error) << "Updatable::UpdateSubTree() : error [" << e.what() << "] by [" << GetTypeName() << "].";
		abort();
	}

	 
	system_clock::time_point currentTime = system_clock::now();
	LOG(LogLevel::Depricated) << "Updatable::UpdateSubTree() : [" << GetTypeName() << "] update cost time = [" << duration_cast<microseconds>(currentTime - startTime).count() << "].";


	LOG(LogLevel::Depricated) << "Updatable::UpdateSubTree() : [" << GetTypeName() << "] try get new child list.";
	cacheChilds.clear();
	unique_lock<mutex> uLock(ChildMutex);
	cacheChilds.assign(GetChilds()->begin(), GetChilds()->end());
	uLock.unlock();
	LOG(LogLevel::Depricated) << "Updatable::UpdateSubTree() : [" << GetTypeName() << "] got new child list.";

	for (vector<ChildAddable*>::iterator iter = cacheChilds.begin(); iter != cacheChilds.end(); iter++) {
		Updatable* child = Cast<Updatable>(*iter);
		if (child != nullptr) {
			LOG(LogLevel::Finest) << "Updatable::UpdateSubTree() : [" << GetTypeName() << "] iterating child [" << (*iter)->GetTypeName() << "] to update.";
			Cast<Updatable>(*iter)->UpdateSubTree();

		}
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



