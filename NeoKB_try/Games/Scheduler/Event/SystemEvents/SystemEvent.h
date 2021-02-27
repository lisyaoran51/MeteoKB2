#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

#include"../Event.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace SystemEvents {

	enum class SystemEventType;

	class SystemEvent : public Event {

	public:
	
		/// <summary>
		/// �ˬd�޿��
		/// </summary>
		SystemEvent();

		SystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType() = 0;

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	protected:




	};

	enum class SystemEventType {
		Start,	// �ΨB�D�A�C���|�۰ʶ}�l
		Pause,	// ����time controller�F
		Resume,	// �ΨB�D
		End,	// �i�Jresult
		Stop,	// �����C�����@��
		Restart
	};

}}}}







#endif