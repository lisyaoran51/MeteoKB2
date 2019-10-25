#ifndef SHEETMUSIC_SELECT_PANEL_H
#define SHEETMUSIC_SELECT_PANEL_H

#include "../../Framework/Allocation/Hierachal/Container.h"



using namespace Framework::Allocation::Hierachal;



namespace Games {
namespace UI {

	class SheetmusicSelectPanel : public Container {

	public:

		SheetmusicSelectPanel();


	protected:

		virtual int onCommand(InputState* inputState, InputKey command);

	};

}}




#endif