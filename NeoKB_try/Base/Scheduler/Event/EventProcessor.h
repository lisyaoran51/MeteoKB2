#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <string>
#include <map>
#include <vector>
#include "../../../Util/TemplateConstraint.h"
#include "../../../Util/MtoType.h"
#include "../../../Util/Update/Updatable.h"
#include "Event.h"


using namespace std;
using namespace Util;
using namespace Util::Update;


namespace Base {
namespace Schedulers {
namespace Events {

	/// <summary>
	/// a processor to process one Event including effects, 
	/// </summary>
	template<class T>
	class EventProcessor : public Updatable, private TConstraint<T, Event>
	{


	public:

		
		/// <summary>
		/// register the Event to be processed.
		/// </summary>
		virtual EventProcessor<T>* RegisterEvent(T* e) {
			event = e;
			return this;
		}

		/// <summary>
		/// the work to do with this Event, such as stop the game, slow down...
		/// 結果應該是用elapse來跑，不試用process??
		/// </summary>
		// virtual int Process() = 0;

		MTO_FLOAT GetStartTime(){ return event->GetStartTime(); }
		MTO_FLOAT GetLifeTime(){ return event->GetLifeTime(); }

		MTO_FLOAT GetTimeLeft(){ 

			if (event->GetLifeType() == EventLifeType::Infinite)
				return MTO_INFINITE;
			else if (event->GetLifeType() == EventLifeType::Immediate)
				return event->GetLifeTime() - currentTime;
			else
				return event->GetLifeTime() - currentTime; 

		}

		// 一定要每次都override!!
		virtual string GetEventTypeName(){ return event->GetTypeName(); }
		
		template<typename U, typename V>
		static bool CanCast(V* e) {
			if (U* t = dynamic_cast< U* >(e)) {
				return true;
			}
			else {
				return false;
			}
		}

		template<typename U, typename V>
		static U* Cast(V* e) {
			return dynamic_cast< U* >(e);
		}

		template<typename U>
		bool CanCast() {
			if (U* t = dynamic_cast< U* >(this)) {
				return true;
			}
			else {
				return false;
			}
		}

		template<typename U>
		U* Cast() {
			return dynamic_cast< U* >(this);
		}


	protected:

		/// <summary>
		/// Event id
		/// </summary>
		int eid;

		T* event;

	};



}}}



#endif