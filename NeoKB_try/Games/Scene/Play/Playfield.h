#ifndef PLAYFIELD_H
#define PLAYFIELD_H


#include "../../Scheduler/Event/EventProcessorMaster.h"
#include "../../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../Scheduler/Event/HitObject.h"
#include "../../Scheduler/Event/IoEvents/IoCommunicators/IoCommunicator.h"



using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Framework::Configurations;
using namespace Framework::Allocation::Hierachal;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;




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

		virtual int OnJudgement(HitObject* hitObject, Judgement* judgement);

		/// <summary>
		/// add events to EventProcessorMaster
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

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

		//Scheduler* scheduler;

		EventProcessorMaster* eventProcessorMaster;

		//Renderer* renderer;

		//Updater* updater;

		//Map* compositeGraph;

		///<summary>
		/// 佔存剛建完的影像，還沒位移到正確位置上
		///</summary>
		Map* bufferMap;

		virtual EventProcessorMaster* createEventProcessorMaster() = 0;

	};


}}}











#endif