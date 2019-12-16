#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include <string>
#include <map>
#include <vector>
#include "../../../Util/TemplateConstraint.h"
#include "../../../Util/MtoType.h"
#include "Event.h"
#include "EventProcessorMaster.h"
#include "../../../Framework/Timing/Clock.h"


using namespace std;
using namespace Util;
using namespace Framework::Timing;


namespace Games {
namespace Schedulers {
namespace Events {

	class EventProcessorMaster;

	enum class EventProcessorLifeType {
		None,
		Timed,
		Immediate,
		Infinite
	};

	/// <summary>
	/// a processor to process one Event including effects, 
	/// </summary>
	template<class T>
	class EventProcessor : public Clock, private TConstraint<T, Event>
	{

		Clock* clock;

		bool isAttached = false;

	public:

		virtual ~EventProcessor() {}
		
		/// <summary>
		/// register the Event to be processed.
		/// </summary>
		virtual EventProcessor<T>* RegisterEvent(T* e) {
			event = e;
			return this;
		}


		/// <summary>
		/// �N�o��processor�[�Jmaster�A�åB����master��clock
		/// </summary>
		int Attach(EventProcessorMaster* master) {
			clock = master->GetClock();
			isAttached = true;
			return 0;
		}

		/* clock */

		virtual double GetCurrentTime() {
			return clock->GetCurrentTime();
		}

		virtual int SetRate(double r) {
			throw logic_error("int EventProcessor::SetRate(double) : no permission to set rate.");
			return 0;
		}
		virtual double GetRate() {
			return clock->GetRate();
		}
		virtual int SetIsRunning(bool value) {
			throw logic_error("int EventProcessor::SetRate(double) : no permission to set IsRunning.");
			return 0;
		}
		virtual bool GetIsRunning() {
			return clock->GetIsRunning();
		}

		/* clock */

		/// <summary>
		/// the work to do with this Event, such as stop the game, slow down...
		/// ���G���ӬO��elapse�Ӷ]�A���ե�process??
		/// </summary>
		// virtual int Process() = 0;

		virtual MTO_FLOAT GetStartTime(){ return event->GetStartTime(); }
		virtual MTO_FLOAT GetLifeTime(){ return event->GetLifeTime(); }

		virtual MTO_FLOAT GetTimeLeft(){ 

			if (event->GetLifeType() == EventLifeType::Infinite)
				return MTO_INFINITE;
			else if (event->GetLifeType() == EventLifeType::Timed)
				return event->GetLifeTime() - (clock->GetCurrentTime() - event->GetStartTime());
			else
				return event->GetLifeTime() - (clock->GetCurrentTime() - event->GetStartTime());

		}

		EventProcessorLifeType GetProcessorLifeType() {
			return lifeType;
		}

		bool GetIsProcessed() {
			return isProcessed;
		}

		/// <summary>
		/// �M�w�o��proccessor�O�_�n�R���C���P��processor���ӭn�����P���R���ɶ��A�����ĪG�ɶ����A�����ĪG�ɶ��u�C
		/// �Ȯɥ��Ω���5���@�R���ɶ��A���L�C��rpocessor���n�ۭq�@�Ӥs���ɶ��~��
		/// </summary>
		virtual MTO_FLOAT GetProcessorTimeLeft() {

			if (lifeType == EventProcessorLifeType::Infinite)
				return MTO_INFINITE;
			else if (lifeType == EventProcessorLifeType::Timed)
				return event->GetLifeTime() + event->GetStartTime() - clock->GetCurrentTime();
			else
				return event->GetLifeTime() + event->GetStartTime() - clock->GetCurrentTime() + 5.0; //?? �o�ӭn���g

		}

		Event* GetEvent() {
			return event;
		}

		// �@�w�n�C����override!!
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

		T* event = nullptr;

		EventProcessorLifeType lifeType = EventProcessorLifeType::None;

		/// <summary>
		/// �bEventProcessorLifeType :: immediate�ƥ�~�|�ϥ�
		/// </summary>
		bool isProcessed = false;

	};

	

}}}



#endif