#ifndef BINDABLE_H
#define BINDABLE_H


#include <vector>
#include "ActionList.h"


/*
 * ����template��default
 * https://stackoverflow.com/questions/12615549/how-to-get-the-default-value-of-any-type
 */


using namespace std;


namespace Util {
namespace DataStructure {

	template<typename T>
	class Bindable {

		mutable mutex bindingMutex;

	public:

		Bindable() {
			
		}

		Bindable(T v) {
			defaultValue = v;
			value = v;
			hasDefaultValue = true;
		}

		virtual ~Bindable() {

			unique_lock<mutex> uLock(bindingMutex);
			if (bindings != nullptr) {
				for (int i = 0; i < bindings->size(); i++) {
					if (bindings->at(i) == this) {
						bindings->erase(bindings->begin() + i);

						break;
					}
				}

				if (bindings->size() == 0) {
					delete bindings;
					bindings = nullptr;
				}

			}
		}

		/// <summary>
		/// �n�P�_�o�ӵ�����pointer�A�ժ��ܤ~��delete
		/// </summary>
		int SetValue(T v, bool deleteLast = false) {

			LOG(LogLevel::Depricated) << "Bindable::SetValue() : ... ";

			if (disabled)
				throw invalid_argument("Bindable<T>::SetValue(): change value is disabled.");
			value = v;
			onValueChanged.Trigger();

			triggerValueChange();
			return 0;
		}

		int SetValueWithoutTrigger(T v, bool deleteLast = false) {
			if (disabled)
				throw invalid_argument("Bindable<T>::SetValue(): change value is disabled.");
			value = v;
			onValueChanged.Trigger();
			return 0;
		}
		
		T GetValue() {
			return value;
		}

		T GetDefaultValue() {
			// TODO: �ˬd���S��default value
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
		/// this BindTo other�O��this����Ƴ��Qother�л\
		/// </summary>
		/// <param name="other">The foreign bindable. This should always be the most permanent end of the bind (ie. a TConfigManager)</param>
		int BindTo(Bindable<T>* other) {

			LOG(LogLevel::Fine) << "Bindable::BindTo() : type = [" << typeid(other).name() << "] binding [" << other << "].";

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
		/// �u�����bind�M�Cbind���ۥ[�A�άO��ӳ��Sbind�L���ۥ[
		/// </summary>
		int AddBindings(Bindable<T>* other) {

			
			if (other->GetBindings() == nullptr) {	// ���S��binding

				LOG(LogLevel::Fine) << "Bindable::AddBindings() : other [" << other << "]'s binding is null. this = [" << this << "].";

				if (bindings == nullptr) {	// �ۤv�]�S��binding
					unique_lock<mutex> uLock(bindingMutex);
					LOG(LogLevel::Fine) << "Bindable::AddBindings() : this [" << this << "] binding is null, so create bindings.";
					bindings = new vector<Bindable<T>*>();
					bindings->push_back(this);
				}
					
				other->AddBindings(this);
			}
			else {	// �ۤv�S��binding

				if (bindings == nullptr) {	// ��観binding
					unique_lock<mutex> uLock(bindingMutex);
					LOG(LogLevel::Fine) << "Bindable::AddBindings() : other [" << other << "]'s binding is [" << other->GetBindings() << "]. this = [" << this << "].";
					bindings = other->GetBindings();
					bindings->push_back(this);
				}
				else {	// ���覭�N�w�gbind�b�@�_
					if (other->GetBindings() == bindings) {
						LOG(LogLevel::Fine) << "Bindable::AddBindings() : these two has already binded.";
					}
					else
						throw logic_error("Bindable<T>::AddBindings(): cannot bind two list of bindings.");
						// ����i�H�g�g�ݨ��bindable list�ۥ[

				}
					
				
			}
			
			return 0;
		}

		template<typename TCallableObject>
		int AddOnValueChanged(TCallableObject object, function<int(void)> oValueChanged, string name) {
			onValueChanged.Add(object, oValueChanged, name);
			return 0;
		}

		template<typename TCallableObject>
		int DeleteOnValueChanged(TCallableObject object) {

			if (bindings == nullptr) {
				onValueChanged.Delete(object);
				return 0;
			}

			for (int i = 0; i < bindings->size(); i++) {
				bindings->at(i)->IterateDeleteOnValueChanged(object);
			}

			return 0;
		}

		template<typename TCallableObject>
		int DeleteOnValueChanged(TCallableObject object, string name) {
			
			if (bindings == nullptr) {
				onValueChanged.Delete(object, name);
				return 0;
			}

			for (int i = 0; i < bindings->size(); i++) {
				bindings->at(i)->IterateDeleteOnValueChanged(object, name);
			}

			return 0;
		}

		/// <summary>
		/// �Ω�R����@��bindable�W����On Value Changed�A�p�G�n�@���R���Ҧ�bindings�W����On Value Changed�A�N�ncall Delete On Value Changed
		/// </summary>
		template<typename TCallableObject>
		int IterateDeleteOnValueChanged(TCallableObject object, string name) {
			onValueChanged.Delete(object, name);
			return 0;
		}

		/// <summary>
		/// �Ω�R����@��bindable�W����On Value Changed�A�p�G�n�@���R���Ҧ�bindings�W����On Value Changed�A�N�ncall Delete On Value Changed
		/// </summary>
		template<typename TCallableObject>
		int IterateDeleteOnValueChanged(TCallableObject object) {
			onValueChanged.Delete(object);
			return 0;
		}

		int UnbindAll() {
			throw runtime_error("Bindable::UnbindAll(): not implemented.");
			return 0;
		}

		int UnBind() {

			unique_lock<mutex> uLock(bindingMutex);
			if (bindings != nullptr) {
				for (int i = 0; i < bindings->size(); i++) {
					if (bindings->at(i) == this) {
						bindings->erase(bindings->begin() + i);
						i--;
					}
				}

				if (bindings->size() == 0)
					delete bindings;

			}

			bindings = nullptr;

			return 0;
		}

	protected:

		/// <summary>
		/// ��s�Ҧ��s����bindings����
		/// </summary>
		int triggerValueChange() {
			LOG(LogLevel::Depricated) << "Bindable::triggerValueChange() : bindings = " << bindings;

			if (bindings == nullptr)
				return 0;
			LOG(LogLevel::Depricated) << "Bindable::triggerValueChange() : start triggering [" << bindings->size() << "] bindings.";

			unique_lock<mutex> uLock(bindingMutex);
			for (int i = 0; i < bindings->size(); i++) {
				LOG(LogLevel::Depricated) << "Bindable::triggerValueChange() : trigger [" << bindings->at(i) << "] binding.";
				bindings->at(i)->SetValueWithoutTrigger(value);
			}
			return 0;
		}

		int triggerDisabledChange() {
			if (bindings = nullptr)
				return 0;

			unique_lock<mutex> uLock(bindingMutex);
			for (int i = 0; i < bindings->size(); i++) {
				bindings->at(i)->SetDisabledWithoutTrigger(disabled);
			}

			return 0;
		}


		T value;
		T defaultValue;
		bool disabled = false;
		bool hasDefaultValue = false;
		ActionList<int> onValueChanged;

		vector<Bindable<T>*>* bindings = nullptr;
		
	};



}}


#endif