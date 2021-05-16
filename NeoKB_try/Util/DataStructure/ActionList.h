#ifndef ACTION_LIST_H
#define ACTION_LIST_H



#include <vector>
#include <functional>
#include <string>
#include <utility>
#include <cstdint>
#include "../Log.h"
#include <mutex>


using namespace std;
using namespace Util;



namespace Util {
namespace DataStructure {

	template<typename _Fty, typename ..._Types>
	class ActionList/*<function<_Fty(_Types...)>>*/ {


		//map<pair<uintptr_t, string>, function<_Fty(_Types...)>> callbackMap;

		vector<pair<uintptr_t, string>> callbackKeys;

		vector<function<_Fty(_Types...)>> callbacks;

		mutable mutex callbackMutex;


	public:

		template<class _Type>
		int Add(_Type* callableObject, function<_Fty(_Types...)> callback, string callbackName) {

			//LOG(LogLevel::Fine) << "ActionList::Add() : register callback into list.";

			//callbackMap[make_pair((uintptr_t)callableObject, callbackName)] = callback;

			return Add((uintptr_t)callableObject, callback, callbackName);
		}

		int Add(uintptr_t callableObjectPointer, function<_Fty(_Types...)> callback, string callbackName) {

			pair<uintptr_t, string> key = make_pair(callableObjectPointer, callbackName);

			LOG(LogLevel::Depricated) << "ActionList::Add() : key = " << key.first << ", name = " << key.second;

			//LOG(LogLevel::Finest) << "ActionList::Add() : key list address:" << &callbackKeys;
			//LOG(LogLevel::Finest) << "ActionList::Add() : key list size:" << callbackKeys.size();

			unique_lock<mutex> uLock(callbackMutex);
			LOG(LogLevel::Depricated) << "ActionList::Add() :1";
			callbackKeys.push_back(key);
			LOG(LogLevel::Depricated) << "ActionList::Add() :2";

			callbacks.push_back(callback);
			LOG(LogLevel::Depricated) << "ActionList::Add() :3";

			return 0;
		}

		int Add(ActionList<_Fty, _Types...>* actionList) {
			for (int i = 0; i < actionList->GetSize(); i++) {
				Add(actionList->GetCallableObjectPointer(i), actionList->GetCallback(i), actionList->GetCallbackName(i));
			}
			return 0;
		}

		uintptr_t GetCallableObjectPointer(int index) {
			return callbackKeys[index].first;
		}

		function<_Fty(_Types...)> GetCallback(int index) {
			return callbacks[index];
		}

		string GetCallbackName(int index) {
			return callbackKeys[index].second;
		}

		template<class _Type>
		int Delete(_Type* callableObject, string callbackName) {
			//map<uintptr_t, string, function<_Fty(_Types...)>>::iterator iter;
			//iter = callbackMap.find(make_pair((uintptr_t)t, func_name));
			//if (iter != callbackMap.end()) {
			//	callbackMap.erase(iter);
			//	return 0;
			//}
			//else {
			//	return -1;
			//}

			pair<uintptr_t, string> toDelete = make_pair((uintptr_t)callableObject, callbackName);

			vector<pair<uintptr_t, string>>::iterator iterKey;
			typename vector<function<_Fty(_Types...)>>::iterator iter;

			unique_lock<mutex> uLock(callbackMutex);
			for (iterKey = callbackKeys.begin(), iter = callbacks.begin();
				iterKey != callbackKeys.end(), iter != callbacks.end();
				++iterKey, ++iter) {

				if (*iterKey == toDelete) {
					callbackKeys.erase(iterKey);
					callbacks.erase(iter);
					return 0;
				}

			}
			return -1;

		}

		/// <summary>
		/// 刪除所有由這個object加入的action
		/// </summary>
		template<class _Type>
		int Delete(_Type* callableObject) {

			uintptr_t pointer = (uintptr_t)callableObject;

			vector<pair<uintptr_t, string>>::iterator iterKey;
			typename vector<function<_Fty(_Types...)>>::iterator iter;

			unique_lock<mutex> uLock(callbackMutex);
			for (iterKey = callbackKeys.begin(), iter = callbacks.begin();
				iterKey != callbackKeys.end(), iter != callbacks.end();
				++iterKey, ++iter) {

				if ((*iterKey).first== pointer) {
					callbackKeys.erase(iterKey);
					callbacks.erase(iter);

					iterKey--;
					iter--;
				}

			}
			return 0;

		}

		int Clear() {
			unique_lock<mutex> uLock(callbackMutex);
			callbackKeys.clear();
			callbacks.clear();

			return 0;
		}

		int Trigger(_Types... _Args) {
			//map<pair<uintptr_t, string>, function<_Fty(_Types...)>>::iterator iter;
			//for (iter = callbackMap.begin(); iter != callbackMap.end(); iter++)
			//	(*(iter->second))(_Args...);
			typename vector<function<_Fty(_Types...)>>::iterator iter;

			unique_lock<mutex> uLock(callbackMutex);
			for (iter = callbacks.begin(); iter != callbacks.end(); ++iter) 
				(*iter)(_Args...);

			return 0;
		}

		int TriggerThenClear(_Types... _Args) {
			// TODO : 要家thread safe
			Trigger(_Args...);
			Clear();

			return 0;
		}

		vector<pair<uintptr_t, string>>* _DebugGetCallbackKeys() {
			return &callbackKeys;
		}

		int GetSize() {
			unique_lock<mutex> uLock(callbackMutex);
			return callbacks.size();
		}

	protected:




	};

}}







#endif
