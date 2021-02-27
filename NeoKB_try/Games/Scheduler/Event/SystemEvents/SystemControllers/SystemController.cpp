#include "SystemController.h"

#include "../SystemEvent.h"


using namespace Games::Schedulers::Events::SystemEvents::SystemControllers;
using namespace Games::Schedulers::Events::SystemEvents;



SystemController::SystemController() : RegisterType("SystemController")
{
}

int SystemController::LazyConstruct(function<int()> lGame, function<int()> rGame, function<int()> eGame)
{
	leaveGame = lGame;
	restartGame = rGame;
	endGame = eGame;
	return 0;
}

int SystemController::ControlSystem(EventProcessor<Event>* eProcessor)
{
	SystemEvent* systemEvent = dynamic_cast<SystemEvent*>(eProcessor);

	if (systemEvent == nullptr)
		return 0;

	if (systemEvent->GetSystemEventType() == SystemEventType::Stop) {
		// TODO: 要不要用add delayed task? 不過都是在update thread裡面的
		leaveGame();
	}
	else if (systemEvent->GetSystemEventType() == SystemEventType::Restart) {
		// TODO: 要不要用add delayed task? 不過都是在update thread裡面的
		restartGame();
	}
	else if (systemEvent->GetSystemEventType() == SystemEventType::End) {
		// TODO: 要不要用add delayed task? 不過都是在update thread裡面的
		endGame();
	}


	return 0;
}
