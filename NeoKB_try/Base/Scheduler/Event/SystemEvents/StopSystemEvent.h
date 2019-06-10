#ifndef STOP_SYSTEM_EVENT_H
#define STOP_SYSTEM_EVENT_H

#include "SystemEvent.h"

using namespace Base::Schedulers::Events;

namespace Base {
namespace Schedulers {
namespace Events {
namespace SystemEvents {


	class StopSystemEvent : public SystemEvent {

	public:
		
		/// <summary>
		/// �ˬd�޿��
		/// </summary>
		StopSystemEvent();
		
		StopSystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

}}}}







#endif