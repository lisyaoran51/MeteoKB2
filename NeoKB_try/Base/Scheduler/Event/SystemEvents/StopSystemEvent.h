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
		/// 檢查邏輯用
		/// </summary>
		StopSystemEvent();
		
		StopSystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType();

		// 一定要每次都override!!
		virtual string GetTypeName();

	};

}}}}







#endif