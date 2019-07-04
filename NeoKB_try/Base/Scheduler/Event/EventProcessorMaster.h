#ifndef EVENT_PROCESSOR_MASTER_H
#define EVENT_PROCESSOR_MASTER_H

#include <string>
#include <map>
#include <vector>
#include "../../../Util/TemplateConstraint.h"
#include "../../../Util/MtoType.h"
#include "../../../Util/InstanceCreator.h"
#include "../../../Util/Update/Updatable.h"
#include "Event.h"
#include "EventProcessor.h"
#include "../../../Framework/Graphic/Drawable.h"
#include "../../../Util/DataStructure/PeriodMap.h"
#include "../../../Framework/Allocation/Hierachal/Triggerable.h"


using namespace std;
using namespace Util::DataStructure;
using namespace Base::Graphic::Maps;
using namespace Util::Update;
using namespace Util::Hierachal;
using namespace Framework::Graphic::Maps;
using namespace Framework::Graphic;
using namespace Framework::Allocation::Hierachal;


/*
* instantiate a class with its name in string
* https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
*/


namespace Base {
namespace Schedulers {
namespace Events {

	/// <summary>
	/// a processor center to process every Event including effects, 
	/// </summary>
	class EventProcessorMaster : public Triggerable, public Drawable
	{
		// for thread
		int tid;

		int load();

		int load(Updater* u);

		float visibleTimeRange = 1.0f;

	public:

		EventProcessorMaster();

		~EventProcessorMaster();

		/// <summary>
		/// �b�C���i�椤�ӭt�d�������N���檺�ƥ�A 
		/// </summary>
		int ReceiveEventProcessor(EventProcessor<Event>* ep);

		/* �令�\�brulset executor��
		/// <summary>
		/// When master receive a Event, he asks the Event's typename. 
		/// Use the typename to register a corresponding processor for this Event.
		/// </summary>
		template<class T>
		int register_event_processor();
		*/

		int RegisterMap(Map* m);

		Map* GetGraph();

		/// <summary>
		/// clean the Event processors
		/// </summary>
		int Clean();

	protected:

		vector<EventProcessor<Event>*> staticEventProcessors;

		vector<EventProcessor<Event>*> dynamicEventProcessors;

		/// <summary>
		/// �j�󵥩�a�A�p��B���ɬq���A���Ҧ�event
		/// </summary>
		PeriodMap<EventProcessor<Event>*>* eventProcessorPeriods;

		int processEvent(MTO_FLOAT elapsedTime);

		int cleanEndedEvent();

		int Elapse(MTO_FLOAT elapsedTime);

		

		virtual int update();

		/* �令�\�brulset executor��
		/// <summary>
		/// a utility to create new processor with specific type 
		/// </summary>
		InstanceCreator<EventProcessor<Event>> event_processor_creator;
		*/

		Map* lightMap;

	};




}}}



#endif