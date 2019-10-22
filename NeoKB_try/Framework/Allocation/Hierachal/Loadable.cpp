#include "Loadable.h"

#include "../../../Util/MtoType.h"

using namespace Framework::Allocation::Hierachal;
using namespace std;
using namespace Util;


/*
 * �h�����祫�\�i�hfunction�̭�����k
 * https://stackoverflow.com/questions/13064698/stdbind-and-overloaded-function
 * https://stackoverflow.com/questions/38559546/stdbind-with-overloaded-function-from-parent-class
 * registerLoad(bind(static_cast<int(??::*)(void)>(&??::load), this));
 */


int Loadable::load()
{
	LOG(LogLevel::Fine) << "int Loadable::load() : [" << GetTypeName() << "] object is loading.";
	for (int i = 0; i < loaders.size(); i++) {
		LOG(LogLevel::Finer) << "int Loadable::load() : [" << GetTypeName() << "] object is loading #" << i << " loader function.";
		loaders[i]();
	}
	return 0;
}

Loadable::Loadable(): RegisterType("Loadable"), Cachable(), noParentHandler(*this), notLoadedHandler(*this), loadingHandler(*this), readyHandler(*this), loadedHandler(*this)
{
	loadStateHandler = &noParentHandler;
}

int Loadable::Async()
{
	LOG(LogLevel::Fine) << "int Loadable::Async() : A [" << GetTypeName() << "] object is asyncing.";
	return loadStateHandler -> Async();
}

int Loadable::LoadAsync(DependencyContainer * dContainer)
{
	SetDependencies(dContainer);
	return loadStateHandler->Async();
}

LoadState Loadable::GetLoadState()
{
	return loadStateHandler->GetLoadState();
}

int Loadable::SetParent(HasParent * p)
{
	loadStateHandler->SetParent(p);
	return 0;
}

int Loadable::registerLoad(function<int(void)> l) {
	// find every parent class's private load() function
	// https://stackoverflow.com/questions/16262338/get-Games-class-for-a-type-in-class-hierarchy
	loaders.push_back(l);
	return 0;
}

int Loadable::Load()
{
	loadStateHandler->HandleLoad();
	return 0;
}

int Loadable::LoadComplete()
{
	loadStateHandler->HandleLoadComplete();
	return 0;
}

int Loadable::LoadOnCompleted()
{
	return 0;
}

int Loadable::setParent(HasParent * p)
{
	return Cachable::SetParent(p);
}


