#include "Cachable.h"
#include <stdexcept>



using namespace Framework::Allocation::Hierachal;


Cachable::Cachable(): RegisterType("Cachable")
{
}

int Cachable::SetParent(HasParent * p)
{
	Cachable* parent = Cast<Cachable>(p);
	if (p != nullptr) {
		dependencies = CreateLocalDependencies(parent->GetDependencies());
	}
	else {
		throw invalid_argument("Cachable::SetParent: parent has to be Cachable.");
	}
	
	return HasParent::SetParent(p);
}

int Cachable::SetDependencies(DependencyContainer * d)
{
	dependencies = d;
	return 0;
}

DependencyContainer * Cachable::GetDependencies()
{
	return dependencies;
}

DependencyContainer * Cachable::CreateLocalDependencies(DependencyContainer * parentDependencies)
{
	// 直接拿parent的dependencies來用
	return parentDependencies;
}




