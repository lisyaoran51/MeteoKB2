#ifndef PANEL_OUTPUTER_H
#define PANEL_OUTPUTER_H


#include "../OutputComponent.h"

using namespace Framework::Output;

namespace Framework {
namespace Output{
namespace Panels{

	class PanelOutputer : virtual public OutputComponent {



	};


	template<typename T>
	class TPanelOutputer : public TOutputComponent<T>, public PanelOutputer {



	};

}}}





#endif
