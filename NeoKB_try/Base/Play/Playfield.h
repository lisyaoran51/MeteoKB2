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

		// �~��playfield��class�A�bload���ɭԤ@�w�n�gŪ��map algo���u�@
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
		/// �b�o��s�F�|�Ψ쪺algo�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�algo
		///</summary>
		map<string, MapAlgorithmInterface*> mapAlgorithms;

		Scheduler* scheduler;

		EventProcessorMaster* eventProcessorMaster;

		Renderer* renderer;

		Updater* updater;

		Map* lightMap;

		///<summary>
		/// ���s��ا����v���A�٨S�첾�쥿�T��m�W
		///</summary>
		Map* bufferMap;
	};


}}











#endif