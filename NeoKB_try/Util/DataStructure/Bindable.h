#ifndef BINDABLE_H
#define BINDABLE_H


#include <vector>
#include "ActionList.h"


/*
 * 怎麼對template用default
 * https://stackoverflow.com/questions/12615549/how-to-get-the-default-value-of-any-type
 */


using namespace std;


namespace Util {
namespace DataStructure {

	template<typename T>
	class Bindable {

	public:

		Bindable() {
			
		}

		Bindable(T v) {
			defaultValue = v;
			value = v;
			hasDefaultValue = true;
		}

		/// <summary>
		/// 要判斷這個視布試pointer，試的話才能delete
		/// </summary>
		int SetValue(T v, bool deleteLast = false) {

			LOG(LogLevel::Debug) << "Bindable::SetValue() : ... ";

			if (disabled)
				throw invalid_argument("Bindable<T>::SetValue(): change value is disabled.");
			value = v;
			onValueChanged.Trigger(v);

			triggerValueChange();
			return 0;
		}

		int SetValueWithoutTrigger(T v, bool deleteLast = false) {
			if (disabled)
				throw invalid_argument("Bindable<T>::SetValue(): change value is disabled.");
			value = v;
			onValueChanged.Trigger(v);
			return 0;
		}
		
		T GetValue() {
			return value;
		}

		T GetDefaultValue() {
			// TODO: 檢查有沒有default value
			return defaultValue;
		}

		int SetDisabled(bool d) {
			disabled = d;
			triggerDisabledChange();
			return 0;
		}

		int SetDisabledWithoutTrigger(bool d) {
			disabled = d;
			return 0;
		}

		bool GetDisabled() {
			return disabled;
		}

		bool GetHasDefaultValue() {
			return hasDefaultValue;
		}

		/// <summary>
		/// Binds outselves to another bindable such that they receive bi-directional updates.
		/// We will take on any value limitations of the bindable we bind width.
		/// this BindTo other是指this的資料都被other覆蓋
		/// </summary>
		/// <param name="other">The foreign bindable. This should always be the most permanent end of the bind (ie. a ConfigManager)</param>
		int BindTo(Bindable<T>* other) {

			LOG(LogLevel::Fine) << "Bindable::BindTo() : type = [" << typeid(other).name() << "] binding.";

			value = other->GetValue();
			disabled = other->GetDisabled();
			hasDefaultValue = other->GetHasDefaultValue();
			
			if (hasDefaultValue) 
				defaultValue = other->GetDefaultValue();

			AddBindings(other);

			return 0;
		}

		vector<Bindable<T>*>* GetBindings() {
			return bindings;
		}

		/// <summary>
		/// 只能夠有bind和媒bind的相加，或是兩個都沒bind過的相加
		/// </summary>
		int AddBindings(Bindable<T>* other) {
			if (other->GetBindings() == nullptr) {

				if (bindings == nullptr) 
					bindings = new vector<Bindable<T>*>();
					
				other->AddBindings(this);
				bindings->push_back(this);
			}
			else {

				if (bindings == nullptr) {
					bindings = other->GetBindings();
					bindings->push_back(this);
				}
				else 
					throw logic_error("Bindable<T>::AddBindings(): cannot bind two list of bindings.");
					// 之後可以寫寫看兩個bindable list相加
				
			}
			
			return 0;
		}

		template<typename TCallableObject>
		int AddOnValueChanged(TCallableObject object, function<int(void*)> oValueChanged, string name) {
			onValueChanged.Add(object, oValueChanged, name);
			return 0;
		}

		int UnbindAll() {
			throw runtime_error("Bindable::UnbindAll(): not implemented.");
			return 0;
		}

		int UnBind() {
			throw runtime_error("Bindable::UnbindAll(): not implemented.");
			return 0;
		}

	protected:

		/// <summary>
		/// 更新所有連接的bindings的值
		/// </summary>
		int triggerValueChange() {
			LOG(LogLevel::Fine) << "Bindable::triggerValueChange() : bindings = " << bindings;

			if (bindings == nullptr)
				return 0;
			LOG(LogLevel::Finer) << "Bindable::triggerValueChange() : start triggering [" << bindings->size() << "] bindings.";

			for (int i = 0; i < bindings->size(); i++) {
				LOG(LogLevel::Finer) << "Bindable::triggerValueChange() : trigger [" << bindings->at(i) << "] binding.";
				bindings->at(i)->SetValueWithoutTrigger(value);
			}
			return 0;
		}

		int triggerDisabledChange() {
			if (bindings = nullptr)
				return 0;

			for (int i = 0; i < bindings->size(); i++) {
				bindings->at(i)->SetDisabledWithoutTrigger(disabled);
			}

			return 0;
		}


		T value;
		T defaultValue;
		bool disabled = false;
		bool hasDefaultValue = false;
		ActionList<int, void*> onValueChanged;

		vector<Bindable<T>*>* bindings = nullptr;
		
	};



}}


#endif