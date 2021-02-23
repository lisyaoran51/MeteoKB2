#ifndef SYSTEM_EVENT_HANDLER_H
#define SYSTEM_EVENT_HANDLER_H


#include "../../../Scheduler/Event/EventProcessor.h"
#include "../../../Scheduler/Event/EventProcessorMaster.h"
#include "SystemEvent.h"
#include "SystemControllers/SystemController.h"
#include <stdexcept>
 #include <unistd.h>
 

using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::SystemEvents::SystemControllers;


namespace Games {
namespace Schedulers {
namespace Events {
namespace SystemEvents {

	class SystemEventHandlerInterface : public EventProcessor<Event>{
		
	public:

		/// <summary>
		/// ���Ӥ��ݭn�S�O�Τ@�hinterface�A�u��mapper�]�ݭn�A���O�o��N���θ�mapper�@�˪����c�N�n
		/// </summary>
		SystemEventHandlerInterface(): EventProcessor<Event>()
		{
		}

		virtual int Process();

		virtual int RegisterSystemController(SystemController* sController) = 0;

		virtual int ControlSystem() = 0;
	};




	template<class T>
	class SystemEventHandler : public SystemEventHandlerInterface, private TConstraint<T, SystemEvent> {

	public:

		/// <summary>
		/// 
		///	</summary>
		SystemEventHandler() : SystemEventHandlerInterface() {
		}

		virtual int RegisterSystemController(SystemController* sController) {
			systemController = sController;
			return 0;
		}

		virtual int Elapse(MTO_FLOAT elapsedTime) {

			//if (!scheduler)
			//	throw runtime_error("int SystemEventHandler::Elapse() : no scheduler registered!");

			//if (GetSystemEvent()->GetSystemEventType() == SystemEventType::Stop) {
			//
			//	system("killall aplay");
			//
			//	usleep(1000000);
			//
			//	system("aplay Resources/Sms/casual_mem.wav");
			//
			//}

			return 0;
		}

		virtual int ControlSystem() {
			return systemController->ControlSystem(this);
		}

		T* GetSystemEvent() { return dynamic_cast<T*>(event); }

	protected:

		SystemController* systemController = nullptr;

	};


}}}}




#endif