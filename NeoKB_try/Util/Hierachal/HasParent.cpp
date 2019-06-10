#include "HasParent.h"


using namespace Util::Hierachal;




HasParent::HasParent(): RegisterType("HasParent")
{
}

bool HasParent::hasParent()
{
	if (parent)
		return true;
	return false;
}

HasParent * HasParent::GetParent()
{
	return parent;
}

int HasParent::SetParent(HasParent * p)
{
	parent = p;
	return 0;
}
