#ifndef MTO_OBJECT_H
#define MTO_OBJECT_H




#include <typeinfo>
#include <string>


using namespace std;

namespace Framework {
namespace Allocation {
namespace Hierachal {

	class RegisterType {

		// TODO: ���L���n���o��h�Ŷ��A��Τ@��lookup table����
		string typeName;

	public:

		string GetTypeName() { return typeName; }

	protected:

		RegisterType(string t) { typeName = t; }

	};

	/// <summary>
	/// ���C��meteo��class�@��type name�A�]���Q���@��reflection���F��A�bprivate load�����
	/// �C�@�ӧڭ̵{���̪�class�@�w���n�мgGetTypeName
	/// </summary>
	class MtoObject : virtual public RegisterType {

	public:

		MtoObject();
		virtual ~MtoObject();

		//virtual ~MtoObject() = default;

		/// <summary>
		/// ����java�̪� (object) as (class)�A�@���૬�覡
		/// </summary>
		template<typename T>
		static T* Cast(MtoObject* o) {
			if (typeid(T*) == typeid(o)) {
				return dynamic_cast<T*>(o);
			}
			else if (T* t = dynamic_cast< T* >(o))
				return dynamic_cast< T* >(o);
			return nullptr;
		}

		/// <summary>
		/// ����java�̪� (object) as (class)�A�@���૬�覡
		/// </summary>
		template<typename T, typename U>
		static T* Cast(U* o) {
			if (T* t = dynamic_cast< T* >(o))
				return dynamic_cast< T* >(o);
			return nullptr;
		}

		template<typename T>
		T* Cast() {
			if (typeid(T*) == typeid(this)) {
				return dynamic_cast<T*>(this);
			}
			else if (T* t = dynamic_cast< T* >(this))
				return dynamic_cast< T* >(this);
			return nullptr;
		}

		/// <summary>
		/// ����java�̪� (object) as (class)�A�@���૬�覡
		/// </summary>
		template<typename T, typename U>
		static bool CanCast(U* o) {
			if (T* t = dynamic_cast< T* >(o)) {
				return true;
			}
			else {
				return false;
			}
		}

	protected:

		bool isDeleting = false;


	};
}}}



#endif