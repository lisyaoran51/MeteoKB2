#ifndef STOP_GAME_EVENT_H
#define STOP_GAME_EVENT_H

#include "GameEvent.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace GameEvents {

	class StopGameEvent : public GameEvent {

	public:

		StopGameEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual GameEventType GetGameEventType();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		virtual Event* Clone();
	};

}}}}







#endif