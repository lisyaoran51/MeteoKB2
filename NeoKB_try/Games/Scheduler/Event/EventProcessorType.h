#ifndef EVENT_PROCESSOR_TYPE_H
#define EVENT_PROCESSOR_TYPE_H



namespace Games {
namespace Schedulers {
namespace Events {

	enum class EventProcessorType {

		None, 
		ControlPoint,
		Effect,
		Instrument,
		Io,
		Playfield,
		Time,
		Recorder

	};

}}}



#endif