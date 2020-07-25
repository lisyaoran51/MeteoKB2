#ifndef PLAYFIELD_H
#define PLAYFIELD_H


#include "../../Scheduler/Event/EventProcessorMaster.h"
#include "../../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../Scheduler/Event/HitObject.h"
#include "../../Scheduler/Event/IoEvents/IoCommunicators/IoCommunicator.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../Scheduler/Event/PlayfieldEvents/PlayfieldControllers/PlayfieldController.h"
#include "../../Scheduler/Event/TimeEvents/TimeControllerControllers/TimeControllerController.h"



using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Framework::Configurations;
using namespace Framework::Allocation::Hierachal;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Games::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Games::Schedulers::Events::TimeEvents::TimeControllerControllers;




namespace Games {
namespace Scenes {
namespace Play {

	/// <summary>
	/// when playfield start, it should connect Scheduler, master, renderer with Updater.
	/// So that these objects could be updated on each frame.
	/// should they be connected in this class????
	/// 其實這個不是scene，應該要移到UI去	
	/// </summary>
	class Playfield: public Container {


		int width, height;

		int load();

		// 繼承playfield的class，在load的時候一定要寫讀取map algo的工作
		int load(FrameworkConfigManager* f);


	public:

		Playfield();
		~Playfield();

		virtual int OnJudgement(HitObject* hitObject, Judgement* judgement);

		/// <summary>
		/// add events to EventProcessorMaster
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

		vector<EventProcessor<Event>*>* GetEventProcessors();

		/// <summary>
		/// add dynamic events to EventProcessorMaster
		/// </summary>
		virtual int AddDynamic(EventProcessor<Event>* ep);

		int GetWidth();

		int GetHeight();

		EventProcessorMaster* GetEventProcessorMaster();

	protected:

		///<summary>
		/// 在這邊存了會用到的algo，在getEventProcessor時可以到這邊選擇要用的algo
		///</summary>
		map<string, MapAlgorithmInterface*> mapAlgorithms;


		///<summary>
		/// 在這邊存了會用到的io communicator，在getEventProcessor時可以到這邊選擇要用的communicator
		///</summary>
		map<string, IoCommunicatorInterface*> ioCommunicators;

		///<summary>
		/// 在這邊存了會用到的instrument controller，在getEventProcessor時可以到這邊選擇要用的controller
		///</summary>
		map<string, InstrumentControllerInterface*> instrumentControllers;

		///<summary>
		/// 在這邊存了會用到的playfield controller，在getEventProcessor時可以到這邊選擇要用的controller
		///</summary>
		map<string, PlayfieldControllerInterface*> playfieldControllers;

		///<summary>
		/// 在這邊存了會用到的time controller controller，在getEventProcessor時可以到這邊選擇要用的controller
		///</summary>
		map<string, TimeControllerControllerInterface*> timeControllerControllers;

		vector<EventProcessor<Event>*> eventProcessors;

		EventProcessorMaster* eventProcessorMaster;

		///<summary>
		/// 佔存剛建完的影像，還沒位移到正確位置上
		///</summary>
		Map* bufferMap;

		virtual EventProcessorMaster* createEventProcessorMaster() = 0;

	};


}}}











#endif