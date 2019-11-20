#include "Bindable.h"
#include <type_traits>


using namespace std;


namespace Util {
namespace DataStructure {

	template<typename T>
	class BindablePointer : protected Bindable<void*> {

	public:

		BindablePointer() {
			if (!is_pointer(T))
				throw logic_error("BindablePointer::BindablePointer(): template is not a pointer.");
			value = nullptr;
		}

		BindablePointer(T p): Bindable<void*>(p) {
			if (!is_pointer(T))
				throw logic_error("BindablePointer::BindablePointer(): template is not a pointer.");
		}

		int SetValue(T v, bool deleteLast = false) {
			return Bindable<void*>::SetValue(v, deleteLast);
		}

		T GetValue() {
			return Bindable<void*>::GetValue();
		}

		T GetDefaultValue() {
			return Bindable<void*>::GetDefaultValue();
		}

		int SetDisabled(bool d) {
			return Bindable<void*>::SetDisabled(d);
		}

		int SetDisabledWithoutTrigger(bool d) {
			return Bindable<void*>::SetDisabledWithoutTrigger(d);
		}

		int SetValueWithoutTrigger(T v, bool deleteLast = false) {
			return Bindable<void*>::SetValueWithoutTrigger(v, deleteLast);
		}

		bool GetDisabled() {
			return Bindable<void*>::GetDisabled();
		}

		bool GetHasDefaultValue() {
			return Bindable<void*>::GetHasDefaultValue();
		}

		int BindTo(BindablePointer<T>* other) {
			return Bindable<void*>::BindTo(other);
		}

		vector<Bindable<T>*>* GetBindings() {
			return Bindable<void*>::GetBindings();
		}

		int AddBindings(BindablePointer<T>* other) {
			return Bindable<void*>::AddBindings(other);
		}

		int UnbindAll() {
			throw runtime_error("Bindable::UnbindAll(): not implemented.");
			return 0;
		}

		int UnBind() {
			throw runtime_error("Bindable::UnbindAll(): not implemented.");
			return 0;
		}



	};

}}