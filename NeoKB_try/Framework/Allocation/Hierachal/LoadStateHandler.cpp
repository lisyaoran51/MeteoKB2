#include "Loadable.h"

#include <stdexcept>
#include "../Log.h"

using namespace Util::Hierachal;
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
	loadable.loadStateHandler = &loadable.loadingHandler;
	loadable.load();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NoParentHandler::SetParent(HasParent * p)
{
	loadable.HasParent::SetParent(p);
	loadable.loadStateHandler = &loadable.notLoadedHandler;
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
	loadable.loadStateHandler = &loadable.loadingHandler;
	loadable.load();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NotLoadedHandler::Async()
{
	LOG(LogLevel::Finer) << "int Loadable::NotLoadedHandler::Async() : [" << loadable.GetTypeName() << "] is asyncing.";
	loadable.loadStateHandler = &loadable.loadingHandler;
	loadable.load();
	loadable.loadStateHandler = &loadable.readyHandler;
	return 0;
}

int Loadable::NotLoadedHandler::SetParent(HasParent * p)
{
	loadable.HasParent::SetParent(p);
	return 0;
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

