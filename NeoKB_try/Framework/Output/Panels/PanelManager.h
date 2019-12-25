#ifndef PANEL_MANAGER_H
#define PANEL_MANAGER_H


#include "../OutputCollectionManager.h"

using namespace Framework::Output;


namespace Framework {
namespace Output{
namespace Panels{

	class PanelManager : public OutputCollectionManager<OutputComponent> {

	public:

		virtual int SetupPeripheral(MainInterface* mainInterface) {}

	};

}}}





#endif
