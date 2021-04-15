#include "Loadable.h"

#include <stdexcept>
#include "../../../Util/Log.h"

using namespace Framework::Allocation::Hierachal;
using namespace Util;

/*
* stdexcept種類
* https://hk.saowen.com/a/180e33be2080d38fcfc0f75820680315c53d2a8035c7be6203cc9a093ff73bc2
*
* inner class怎麼取外面class的値
* https://stackoverflow.com/questions/486099/can-inner-classes-access-private-variables
*/

Loadable::LoadStateHandler::LoadStateHandler(Loadable & l): loadable(l)
{
}

// --------------------------

Loadable::NoParentHandler::NoParentHandler(Loadable & l): LoadStateHandler(l)
{
}

LoadState Loadable::NoParentHandler::GetLoadState()
{
	return LoadState::NoParent;
}

int Loadable::NoParentHandler::HandleLoad()
{
	throw runtime_error("int Loadable::Load(): This has no parent yet.");
	return -1;
}

int Loadable::NoParentHandler::Async()
{
	LOG(LogLevel::Finer) << "int Loadable::NoParentHandler::Async() : [" << loadable.GetTypeName() << "] is asyncing.";
	
	if (loadable.GetDependencies() == nullptr) {
		LOG(LogLevel::Error) << "int Loadable::NoParentHandler::Async() : There's no dependency in this object. Unable to async";
		throw runtime_error("int Loadable::NoParentHandler::Async() : There's no dependency in this object. Unable to async.");
	}
	unique_lock<mutex> uLock(loadable.stateHandlerMutex);
	loadable.loadStateHandler = &loadable.loadingHandler;
	uLock.unlock();
	loadable.load();

	uLock.lock();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NoParentHandler::SetParent(HasParent * p)
{
	loadable.setParent(p);
	unique_lock<mutex> uLock(loadable.stateHandlerMutex);
	loadable.loadStateHandler = &loadable.notLoadedHandler;
	return 0;
}

int Loadable::NoParentHandler::HandleLoadComplete()
{
	return 0;
}

// --------------------------

Loadable::NotLoadedHandler::NotLoadedHandler(Loadable & l) : LoadStateHandler(l)
{
}

LoadState Loadable::NotLoadedHandler::GetLoadState()
{
	return LoadState::NotLoaded;
}

int Loadable::NotLoadedHandler::HandleLoad()
{
	unique_lock<mutex> uLock(loadable.stateHandlerMutex);
	loadable.loadStateHandler = &loadable.loadingHandler;
	uLock.unlock();
	loadable.load();
	uLock.lock();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NotLoadedHandler::Async()
{
	LOG(LogLevel::Finer) << "int Loadable::NotLoadedHandler::Async() : [" << loadable.GetTypeName() << "] is asyncing.";
	unique_lock<mutex> uLock(loadable.stateHandlerMutex);
	loadable.loadStateHandler = &loadable.loadingHandler;
	uLock.unlock();
	loadable.load();
	uLock.lock();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NotLoadedHandler::SetParent(HasParent * p)
{
	loadable.setParent(p);
	return 0;
}

int Loadable::NotLoadedHandler::HandleLoadComplete()
{
	return -1;
}

// --------------------------

Loadable::LoadingHandler::LoadingHandler(Loadable & l) : LoadStateHandler(l)
{
}

LoadState Loadable::LoadingHandler::GetLoadState()
{
	return LoadState::Loading;
}

int Loadable::LoadingHandler::HandleLoad()
{
	// 讀取中，不能重複讀取
	return -1;
}

int Loadable::LoadingHandler::Async()
{
	// 讀取中，不能重複讀取
	return -1;
}

int Loadable::LoadingHandler::SetParent(HasParent * p)
{
	// 已讀取中
	return -1;
}

int Loadable::LoadingHandler::HandleLoadComplete()
{
	return -1;
}

// --------------------------

Loadable::ReadyHandler::ReadyHandler(Loadable & l) : LoadStateHandler(l)
{
}

LoadState Loadable::ReadyHandler::GetLoadState()
{
	return LoadState::Ready;
}

int Loadable::ReadyHandler::HandleLoad()
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::ReadyHandler::Async()
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::ReadyHandler::SetParent(HasParent * p)
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::ReadyHandler::HandleLoadComplete()
{
	LOG(LogLevel::Debug) << "Loadable::ReadyHandler::HandleLoadComplete() : [" << loadable.GetTypeName() << "] load on clomplete.";

	loadable.LoadOnComplete();
	
	LOG(LogLevel::Fine) << "Loadable::ReadyHandler::HandleLoadComplete() : changing to loaded handler.";

	unique_lock<mutex> uLock(loadable.stateHandlerMutex);
	loadable.loadStateHandler = &loadable.loadedHandler;
	
	return 0;
}

// --------------------------

Loadable::LoadedHandler::LoadedHandler(Loadable & l) : LoadStateHandler(l)
{
}

LoadState Loadable::LoadedHandler::GetLoadState()
{
	return LoadState::Loaded;
}

int Loadable::LoadedHandler::HandleLoad()
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::LoadedHandler::Async()
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::LoadedHandler::SetParent(HasParent * p)
{
	// TODO: 要寫重複利用的方法
	return -1;
}

int Loadable::LoadedHandler::HandleLoadComplete()
{
	return 0;
}

