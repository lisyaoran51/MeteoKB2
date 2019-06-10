#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

#include"../Event.h"

using namespace Base::Schedulers::Events;

namespace Base {
namespace Schedulers {
namespace Events {
namespace SystemEvents {

	enum class SystemEventType;

	class SystemEvent : public Event {

	public:
	
		/// <summary>
		/// 檢查邏輯用
		/// </summary>
		SystemEvent();

		SystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType() = 0;

		// 一定要每次都override!!
		virtual string GetTypeName();


	};

	enum class SystemEventType {
		Start,
		Pause,
		Resume,
		End,
		Stop
	};

}}}}







#endif