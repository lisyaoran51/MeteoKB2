#ifndef RESTART_SYSTEM_EVENT_H
#define RESTART_SYSTEM_EVENT_H

#include "SystemEvent.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace SystemEvents {


	class RestartSystemEvent : public SystemEvent {

	public:
		
		/// <summary>
		/// 檢查邏輯用
		/// </summary>
		RestartSystemEvent();
		
		RestartSystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType();

		// 一定要每次都override!!
		virtual string GetTypeName();

	};

}}}}







#endif