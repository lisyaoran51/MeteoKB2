#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H

#include"../Event.h"

using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace ControlPoints {

	enum class ControlPointType;

	class ControlPoint : public Event {

	public:

		ControlPoint(MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType() = 0;

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	};

	enum class ControlPointType {
		Note,
		Input,
		Sound,
		Effect
	};

}}}}

#endif