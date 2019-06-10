#ifndef TEMPLATE_CONSTRAINT_T
#define TEMPLATE_CONSTRAINT_T

#include <iostream>
#include "Log.h"

/*
 * http://www.stroustrup.com/bs_faq2.html#constraints
 * make a constraint for template
 *
 * awesome trick to automatically register class
 * https://stackoverflow.com/questions/10332725/how-to-automatically-register-a-class-on-creation
 */

using namespace std;


namespace Util {

	/// <summary>
	/// make a constraint for template
	/// </summary>
	template<class T_derived, class T_constraint>
	class TConstraint {
	
		/// <summary>
		/// to check if the template class is derived from T_constraint
		/// </summary>
		//template<class T_derived>
		//void constraint_check_derived_from() {
		//	T_derived* d;
		//	TConstraint* c = d;
		//}

		/// <summary>
		/// to check if the template class is derived from T_constraint
		/// </summary>
		static void constraint_check_derived_from() {
			// TODO: 有問題，暫時放棄
			LOG(LogLevel::Error) << "void TConstraint::constraint_check_derived_from() : not available function.";
			T_derived* d = new T_derived;
			T_constraint* c = d;
		}

		/// <summary>
		/// used to automatically check
		/// https://stackoverflow.com/questions/10332725/how-to-automatically-register-a-class-on-creation
		/// </summary>
		static bool check;

		static bool init()
		{
			if(false)
				constraint_check_derived_from();
			return true;
		}

	public:

		/// <summary>
		/// never want it really to check. just compile constraint_check_derived_from() is enough.
		/// </summary>
		TConstraint(){
			if(false)
				check;
		}

		/// <summary>
		/// make a constraint for template. User have to design his own checking workflow. 
		/// 有需要的時候再覆寫
		/// </summary>
		virtual void constraint_check() {}

		

	};

	template<class T_derived, class T_constraint>
	bool TConstraint<T_derived, T_constraint>::check = init();

}


#endif