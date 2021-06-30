#ifndef EVENT_PROCESSOR_MASTER_H
#define EVENT_PROCESSOR_MASTER_H

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "../../../Util/TemplateConstraint.h"
#include "../../../Util/MtoType.h"
#include "../../../Util/InstanceCreator.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "Event.h"
#include "EventProcessorFilter.h"
#include "../../../Instruments/MeteoPianoPitchState.h"
#include "../../../Framework/Graphic/Drawable.h"
#include "../../../Util/DataStructure/PeriodMap.h"
#include "../../../Framework/Allocation/Hierachal/Triggerable.h"
#include "../../../Framework/Timing/FramedClock.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../../Framework/Output/OutputManager.h"
#include "../../../Framework/Timing/TimeController.h"



using namespace std;
using namespace Util::DataStructure;
using namespace Instruments;
using namespace Framework::Graphic::Maps;
using namespace Framework::Graphic;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Timing;
using namespace Framework::Configurations;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Output;




/*
* instantiate a class with its name in string
* https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
*/


namespace Games {
namespace Schedulers {
namespace Events {

	template<class T>
	class EventProcessor;

	/// <summary>
	/// a processor center to process every Event including effects, 
	/// </summary>
	class EventProcessorMaster : public Container, public Drawable
	{
		// for thread
		int tid;

		int load();

		int load(FrameworkConfigManager* f, EventProcessorFilter* e, OutputManager* o, TimeController* t);

		bool isFirstUpdate = true;
	public:

		EventProcessorMaster();

		virtual ~EventProcessorMaster();

		int AddStaticEventProcessor(EventProcessor<Event>* sEventProcessor);

		int AddDynamicEventProcessor(EventProcessor<Event>* dEventProcessor);

		/* 改成擺在rulset executor裡
		/// <summary>
		/// When master receive a Event, he asks the Event's typename. 
		/// Use the typename to register a corresponding processor for this Event.
		/// </summary>
		template<class T>
		int register_event_processor();
		*/

		Map* GetGraph();

		/// <summary>
		/// clean the Event processors
		/// </summary>
		int Clean();

		PeriodMap<EventProcessor<Event>*>* GetEventProcessorPeriods();

		int ChangePitchState(MeteoPianoPitchState pState);

		template<class _Type>
		int AddOnRetry(_Type* callableObject, function<int()> callback, string name = "HandleRetryRequest") {
			if (timeController)
				timeController->AddOnRetry(callableObject, callback, name);
			else
				LOG(LogLevel::Error) << "EventProcessorMaster::AddOnRetry() : not time controller. failed.";
			return 0;
		}

		template<class _Type>
		int AddOnQuit(_Type* callableObject, function<int()> callback, string name = "HandleQuitRequest") {
			if (timeController)
				timeController->AddOnQuit(callableObject, callback, name);
			else
				LOG(LogLevel::Error) << "EventProcessorMaster::AddOnQuit() : not time controller. failed.";
			return 0;
		}

		template<class _Type>
		int AddOnGameOver(_Type* callableObject, function<int()> callback, string name = "HandleGameOverRequest") {
			if (timeController)
				timeController->AddOnGameOver(callableObject, callback, name);
			else
				LOG(LogLevel::Error) << "EventProcessorMaster::AddOnGameOver() : not time controller. failed.";
			return 0;
		}

	protected:

		float visibleTimeRange = 1.0f;

		mutex processorsMutex;

		//Map* lightMap;

		vector<EventProcessor<Event>*> staticEventProcessors;

		vector<EventProcessor<Event>*> dynamicEventProcessors;

		/// <summary>
		/// 大於等於a，小於B的時段內，的所有event
		/// </summary>
		PeriodMap<EventProcessor<Event>*>* eventProcessorPeriods;

		EventProcessorFilter* eventProcessorFilter = nullptr;

		vector<EventProcessor<Event>*> filteredTempStaticEventProcessors;

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		OutputManager* outputManager = nullptr;

		TimeController* timeController = nullptr;

		/// <summary>
		/// 用來判斷是不是暫停狀態，暫停中就不能夠判斷分數
		/// </summary>
		bool isGamePaused = false;

		int processEvent(MTO_FLOAT elapsedTime);

		

		virtual int update();
	};

	/// <summary>
	/// Event prossor master加上key binding handler
	/// </summary>
	template<typename T>
	class TEventProcessorMaster : public EventProcessorMaster, public KeyBindingHandler<T> {

	public:

		TEventProcessorMaster(): RegisterType("TEventProcessorMaster"){}

		virtual Pitch GetPitchFromAction(T action) = 0;

	protected:



	};

}}}



#endif