#ifndef PLAYFIELD_H
#define PLAYFIELD_H


#include"../../Util/Update/Updatable.h"
#include"../Scheduler/Event/Event.h"
#include"../Scheduler/Event/EventProcessor.h"
#include"../Scheduler/Event/EventProcessorMaster.h"
#include"../Scheduler/Scheduler.h"
#include "../../Util/Hierachal/ChildAddable.h"
#include "../Graphic/Renderer/renderer.h"
#include "../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include"../../Util/Update/Updater.h"



using namespace Util::Hierachal;
using namespace Util::Update;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers;
using namespace Base::Graphic::Renderers;
using namespace Base::Schedulers::Events::Effects::Algorithms;



namespace Base {
namespace Play {

	/// <summary>
	/// when playfield start, it should connect Scheduler, master, renderer with Updater.
	/// So that these objects could be updated on each frame.
	/// should they be connected in this class????
	/// </summary>
	class Playfield: public Updatable, public ChildAddable {

		int load();

		// 繼承playfield的class，在load的時候一定要寫讀取map algo的工作
		int load(FrameworkConfigManager* f, Updater* u);

	public:

		Playfield();

		//virtual int OnJudgement(Event* judge_event, Judgement* j);

		/// <summary>
		/// add events to scheuler
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

		int GetWidth();

		int GetHeight();

	protected:

		int width, height;

		///<summary>
		/// 在這邊存了會用到的algo，在getEventProcessor時可以到這邊選擇要用的algo
		///</summary>
		map<string, MapAlgorithmInterface*> mapAlgorithms;

		Scheduler* scheduler;

		EventProcessorMaster* eventProcessorMaster;

		Renderer* renderer;

		Updater* updater;

		Map* lightMap;

		///<summary>
		/// 佔存剛建完的影像，還沒位移到正確位置上
		///</summary>
		Map* bufferMap;
	};


}}











#endif