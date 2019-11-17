#include "PanelInputHandler.h"

#include "../../IO/Panel.h"


using namespace Framework::Input::Handler;
using namespace Framework::IO;




int PanelInputHandler::Initialize(GameHost * host)
{
	LOG(LogLevel::Info) << "PanelInputHandler::Initialize() : adding handler.";

	// 當mian interface讀到輸入時，就會call handle key down來處理
	host->GetMainInterface()->GetPanel()->
		AddOnPanelEvent<PanelInputHandler>(this, bind((int(PanelInputHandler::*)(InputState*))&PanelInputHandler::HandleState, this, placeholders::_1), "PanelInputHandler::HandleState");

	return 0;
}

int PanelInputHandler::HandleState(InputState * inputState)
{
	auto panelState = inputState->GetPanelState()->Clone();

	InputState* pendingState = new InputState();
	pendingState->SetPanelState(panelState);

	pendingStates.push_back(pendingState);

	return 0;
}
