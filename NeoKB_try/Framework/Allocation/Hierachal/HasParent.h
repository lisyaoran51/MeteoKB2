#ifndef HAS_PARENT_H
#define HAS_PARENT_H


#include "MtoObject.h"





namespace Framework {
namespace Allocation {
namespace Hierachal {

	/// <summary>
	/// has a parent object
	/// </summary>
	class HasParent: virtual public MtoObject {

		HasParent* parent = nullptr;

	public:

		HasParent();

		virtual ~HasParent() = default;

		bool hasParent();

		HasParent* GetParent();

		virtual int SetParent(HasParent* p);

	};




}
}
}



#endif