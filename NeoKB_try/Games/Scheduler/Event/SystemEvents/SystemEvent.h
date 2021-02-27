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
		/// 檢查邏輯用
		/// </summary>
		SystemEvent();

		SystemEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual SystemEventType GetSystemEventType() = 0;

		// 一定要每次都override!!
		virtual string GetTypeName();

	protected:




	};

	enum class SystemEventType {
		Start,	// 用步道，遊戲會自動開始
		Pause,	// 移到time controller了
		Resume,	// 用步道
		End,	// 進入result
		Stop,	// 結束遊戲的一死
		Restart
	};

}}}}







#endif