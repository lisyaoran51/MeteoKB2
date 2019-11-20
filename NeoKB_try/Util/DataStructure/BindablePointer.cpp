#include "BindablePointer.h"



using namespace Util::DataStructure;



BindablePointer::BindablePointer()
{
	value = nullptr;
	defaultValue = nullptr;
}

BindablePointer::BindablePointer(void* p): Bindable(p){}
