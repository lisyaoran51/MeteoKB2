#ifndef STOP_GAME_EVENT_H
#define STOP_GAME_EVENT_H

#include "GameEvent.h"

using namespace Base::Schedulers::Events;

namespace Base {
namespace Schedulers {
namespace Events {
namespace GameEvents {

	class StopGameEvent : public GameEvent {

	public:

		StopGameEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual GameEventType GetGameEventType();

		// 一定要每次都override!!
		virtual string GetTypeName();

		virtual Event* Clone();
	};

}}}}







#endif