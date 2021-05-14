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
using namespace Framework::Graphic::Maps;
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

		/* �令�\�brulset executor��
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

	protected:

		float visibleTimeRange = 1.0f;

		mutex processorsMutex;

		//Map* lightMap;

		vector<EventProcessor<Event>*> staticEventProcessors;

		vector<EventProcessor<Event>*> dynamicEventProcessors;

		/// <summary>
		/// �j�󵥩�a�A�p��B���ɬq���A���Ҧ�event
		/// </summary>
		PeriodMap<EventProcessor<Event>*>* eventProcessorPeriods;

		EventProcessorFilter* eventProcessorFilter = nullptr;

		vector<EventProcessor<Event>*> filteredTempStaticEventProcessors;

		OutputManager* outputManager = nullptr;

		TimeController* timeController = nullptr;

		/// <summary>
		/// �ΨӧP�_�O���O�Ȱ����A�A�Ȱ����N������P�_����
		/// </summary>
		bool isGamePaused = false;

		int processEvent(MTO_FLOAT elapsedTime);

		

		virtual int update();

		/* �令�\�brulset executor��
		/// <summary>
		/// a utility to create new processor with specific type 
		/// </summary>
		InstanceCreator<EventProcessor<Event>> event_processor_creator;
		*/

		// ����meteor event processor master�̭��F
		virtual int onKeyDown(InputState* inputState, Key key);

	};

	/// <summary>
	/// Event prossor master�[�Wkey binding handler
	/// </summary>
	template<typename T>
	class TEventProcessorMaster : public EventProcessorMaster, public KeyBindingHandler<T> {

	public:

		TEventProcessorMaster(): RegisterType("TEventProcessorMaster"){}
	};

}}}



#endif