#ifndef ACTION_MAP_H
#define ACTION_MAP_H



#include <map>
#include <functional>
#include <string>
#include <utility>
#include <cstdint>

using namespace std;


namespace Util {
namespace DataStructure {

	template<typename _Fty, typename ..._Types>
	class ActionMap/*<function<_Fty(_Types...)>>*/ {


		map<pair<uintptr_t, string>, function<_Fty(_Types...)>> callbackMap;

	public:

		template<class _Type>
		int Add(_Type* callableObject, function<_Fty(_Types...)> callback, string callbackName) {
			callbackMap[make_pair((uintptr_t)callableObject, callbackName)] = callback;
			return 0;
		}

		template<class _Type>
		int Delete(_Type* callableObject, string callbackName) {
			typename map<uintptr_t, string, function<_Fty(_Types...)>>::iterator iter;
			iter = callbackMap.find(make_pair((uintptr_t)t, func_name));
			if (iter != callbackMap.end()) {
				callbackMap.erase(iter);
				return 0;
			}
			else {
				return -1;
			}
		}

		int Clear() {
			callbackMap.clear();

			return 0;
		}

		int Trigger(_Types... _Args) {
			typename map<pair<uintptr_t, string>, function<_Fty(_Types...)>>::iterator iter;
			for (iter = callbackMap.begin(); iter != callbackMap.end(); iter++)
				(*(iter->second))(_Args...);
			return 0;
		}

		int TriggerThenClear(_Types... _Args) {
			// TODO : ­n®athread safe
			Trigger(_Args);
			Clear();

			return 0;
		}

	protected:




	};

}}







#endif
