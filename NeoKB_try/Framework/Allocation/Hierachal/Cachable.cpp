#include "Cachable.h"
#include <stdexcept>



using namespace Util::Hierachal;


Cachable::Cachable(): RegisterType("Cachable")
{
}

MtoObject* Cachable::getCache(string type)
{
	
	if (cache.find(type) != cache.end()) {
		// §ä¨ì

		MtoObject* c = cache[type];
		return c;
	}
	return NULL;
}



