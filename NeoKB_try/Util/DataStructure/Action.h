#ifndef ACTION_H
#define ACTION_H



#include <map>
#include <functional>
#include <string>
#include <utility>
#include <cstdint>

using namespace std;


namespace Util {
namespace DataStructure {

	template<typename _Fty, typename ..._Types>
	class Action/*<function<_Fty(_Types...)>>*/ {

		

	public:

		template<class _Type>
		int Add(_Type* callableObject, function<_Fty(_Types...)> callback, string callbackName);

		template<class _Type>
		int Delete(_Type* callableObject, string callbackName);

		int Clear();

		int Trigger(_Types... _Args);

		int TriggerThenClear(_Types... _Args);

	protected:


	private:


		map<pair<uintptr_t, string>, function<_Fty(_Types...)>> callbackMap;


	};

}}







#endif
