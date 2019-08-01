#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include "Event/Event.h"
#include "Event/EventProcessor.h"
#include <functional>
#include "../../Util/Hierachal/ChildAddable.h"
#include "../../Util/Update/Updater.h"
#include "../../Util/Update/Updatable.h"
#include "../../Util/MtoType.h"


using namespace std;
using namespace Base::Schedulers::Events;
using namespace Util::Hierachal;
using namespace Util::Update;


namespace Base {
namespace Schedulers {

	/// <summary>
	/// to watch if a timing task is up
	/// </summary>
	class Scheduler: public Updatable, public ChildAddable {

		int tid;

		vector<EventProcessor<Event>*>* eventProcessors;

		/// <summary>
		/// let the processor master to register the handler for every Event
		/// ---(¤£¥Î¤F)int (*deliverHandler)(EventProcessor<Event>* ep);
		/// </summary>
		function<int(EventProcessor<Event>*)> deliverHandler;

		int load();

		int load(Updater* u);

	public:

		Scheduler();

		~Scheduler();

		int Add(EventProcessor<Event>* ep);

		int AddRange(vector<EventProcessor<Event>*>* eps);

		int RegisterHandler(function<int(EventProcessor<Event>*)> dh);

		virtual int Elapse(MTO_FLOAT elapsedTime);
	};

}}

#endif