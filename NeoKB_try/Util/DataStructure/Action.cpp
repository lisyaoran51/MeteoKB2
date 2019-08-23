#include "Action.h"



using namespace Util::DataStructure;
using namespace std;




/// <summary>
/// 
/// 呼叫方式：myfunc.regp(&a, bind(&testtt::func_obj5, &a, placeholders::_1), "func_obj5");
///                  add  物件     函式                                        函式名稱
///
///	</summary>
template<typename _Fty, typename ..._Types>
template<class _Type>
int Action<_Fty, _Types...>::Add(_Type * callableObject, function<_Fty(_Types...)> callback, string callbackName)
{
	callbackMap[make_pair((uintptr_t)callableObject, callbackName)] = callback;
	return 0;
}

template<typename _Fty, typename ..._Types>
template<class _Type>
int Action<_Fty, _Types...>::Delete(_Type * callableObject, string callbackName)
{
	map<uintptr_t, string, function<T(Args...)>>::iterator iter;
	iter = callbackMap.find(make_pair((uintptr_t)t, func_name));
	if (iter != callbackMap.end()) {
		callbackMap.erase(iter);
		return 0;
	}
	else {
		return -1;
	}
}

template<typename _Fty, typename ..._Types>
int Action<_Fty, _Types...>::Clear()
{
	callbackMap.clear();

	return 0;
}

template<typename _Fty, typename ..._Types>
int Action<_Fty, _Types...>::Trigger(_Types ..._Args)
{
	map<pair<uintptr_t, string>, function<T(Args...)>>::iterator iter;
	for (iter = callbackMap.begin(); iter != callbackMap.end(); iter++)
		(*(iter->second))(_Args...);
	return 0;
}

template<typename _Fty, typename ..._Types>
int Action<_Fty, _Types...>::TriggerThenClear(_Types ..._Args)
{
	// TODO : 要家thread safe
	Trigger(_Args);
	Clear();

	return 0;
}

/*
template<typename _Fty, typename ..._Types>
int Action<function<_Fty(_Types...)>>::Trigger(_Types ..._Args)
{
	map<pair<uintptr_t, string>, function<T(Args...)>>::iterator iter;
	for (iter = callbackMap.begin(); iter != callbackMap.end(); iter++)
		(*(iter->second))(_Args...);
	return 0;
}
*/
