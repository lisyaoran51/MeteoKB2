#include "SheetmusicSelectPanel.h"


using namespace Games::UI;



SheetmusicSelectPanel::SheetmusicSelectPanel(): RegisterType("SheetmusicSelectPanel")
{
	isInputable = true;
}

int SheetmusicSelectPanel::onCommand(InputState * inputState, InputKey command)
{
	return 0;
}
