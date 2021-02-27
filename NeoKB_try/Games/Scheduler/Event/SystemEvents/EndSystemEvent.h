#ifndef END_SYSTEM_EVENT_H
#define END_SYSTEM_EVENT_H

#include "SystemEvent.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace SystemEvents {


	class EndSystemEvent : public SystemEvent {

	public:
		
		/// <summary>
		/// �ˬd�޿��
		/// </summary>
		EndSystemEvent();
		
		EndSystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

}}}}







#endif