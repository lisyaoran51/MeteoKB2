#ifndef ACTION_LIST_H
#define ACTION_LIST_H



#include <vector>
#include <functional>
#include <string>
#include <utility>
#include <cstdint>
#include "../Log.h"


using namespace std;
using namespace Util;



namespace Util {
namespace DataStructure {

	template<typename _Fty, typename ..._Types>
	class ActionList/*<function<_Fty(_Types...)>>*/ {


		//map<pair<uintptr_t, string>, function<_Fty(_Types...)>> callbackMap;

		vector<pair<uintptr_t, string>> callbackKeys;

		vector<function<_Fty(_Types...)>> callbacks;

	public:

		template<class _Type>
		int Add(_Type* callableObject, function<_Fty(_Types...)> callback, string callbackName) {

			LOG(LogLevel::Fine) << "ActionList::Add() : register callback into list.";

			//callbackMap[make_pair((uintptr_t)callableObject, callbackName)] = callback;

			callbackKeys.push_back(make_pair((uintptr_t)callableObject, callbackName));
			LOG(LogLevel::Finest) << "ActionList::Add() : middle.";
			callbacks.push_back(callback);

			LOG(LogLevel::Finest) << "ActionList::Add() : end.";

			return 0;
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

		int Clear() {
			callbackKeys.clear();
			callbacks.clear();

			return 0;
		}

		int Trigger(_Types... _Args) {
			//map<pair<uintptr_t, string>, function<_Fty(_Types...)>>::iterator iter;
			//for (iter = callbackMap.begin(); iter != callbackMap.end(); iter++)
			//	(*(iter->second))(_Args...);
			typename vector<function<_Fty(_Types...)>>::iterator iter;

			for (iter = callbacks.begin(); iter != callbacks.end(); ++iter) 
				(*iter)(_Args...);

			return 0;
		}

		int TriggerThenClear(_Types... _Args) {
			// TODO : ­n®athread safe
			Trigger(_Args...);
			Clear();

			return 0;
		}

	protected:




	};

}}







#endif
