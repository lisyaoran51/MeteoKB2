#ifndef SYSTEM_EVENT_HANDLER_H
#define SYSTEM_EVENT_HANDLER_H


#include "../../../Scheduler/Event/EventProcessor.h"
#include "../../../Scheduler/Event/EventProcessorMaster.h"
#include "SystemEvent.h"
#include "../../Scheduler.h"
#include <stdexcept>
 #include <unistd.h>
 

using namespace Base::Schedulers::Events;
using namespace Base::Schedulers;


namespace Base {
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

		/// <summary>
		/// ��updater����
		/// </summary>
		virtual int RegisterUpdater (Updater* u) = 0;


	};




	template<class T>
	class SystemEventHandler : public SystemEventHandlerInterface, private TConstraint<T, SystemEvent> {

	public:

		/// <summary>
		/// 
		///	</summary>
		SystemEventHandler() : SystemEventHandlerInterface() {
		}

		virtual int Elapse(MTO_FLOAT elapsedTime) {

			//if (!scheduler)
			//	throw runtime_error("int SystemEventHandler::Elapse() : no scheduler registered!");

			if (GetSystemEvent()->GetSystemEventType() == SystemEventType::Stop) {

				system("killall aplay");

				usleep(1000000);

				system("aplay Resources/Sms/casual_mem.wav");

			}

			return 0;
		}

		/// <summary>
		/// 
		/// </summary>
		virtual int RegisterUpdater(Updater* u) {
			updater = u;
		}

		T* GetSystemEvent() { return dynamic_cast<T*>(event); }

	protected:

		Updater* updater;

	};


}}}}




#endif