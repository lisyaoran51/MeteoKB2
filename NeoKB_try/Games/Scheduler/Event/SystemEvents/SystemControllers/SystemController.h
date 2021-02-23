#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H


#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../Scheduler/Event/EventProcessor.h"


namespace Games {
namespace Schedulers{
namespace Events{
namespace SystemEvents{
namespace SystemControllers{

	class SystemController : public Container {

	public:

		SystemController();

		virtual int LazyConstruct(function<int()> lGame, function<int()> rGame);

		virtual int ControlSystem(EventProcessor<Event>* eProcessor);

	protected:

		function<int()> leaveGame;

		function<int()> restartGame;


	};



}}}}}








#endif