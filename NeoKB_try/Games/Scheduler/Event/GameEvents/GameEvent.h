#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include"../Event.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace GameEvents {

	enum class GameEventType;

	class GameEvent : public Event {

	public:

		GameEvent(MTO_FLOAT s, MTO_FLOAT l);

		virtual GameEventType GetGameEventType() = 0;

		// �@�w�n�C����override!!
		virtual string GetTypeName();


	};

	enum class GameEventType {
		Start,
		Pause,
		Resume,
		End,
		Stop
	};

}}}}







#endif