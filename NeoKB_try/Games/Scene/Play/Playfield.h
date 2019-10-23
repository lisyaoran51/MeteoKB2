#ifndef PLAYFIELD_H
#define PLAYFIELD_H


#include "../../Scheduler/Event/EventProcessorMaster.h"
#include "../../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"



using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Framework::Configurations;
using namespace Framework::Allocation::Hierachal;



namespace Games {
namespace Scenes {
namespace Play {

	/// <summary>
	/// when playfield start, it should connect Scheduler, master, renderer with Updater.
	/// So that these objects could be updated on each frame.
	/// should they be connected in this class????
	/// ���o�Ӥ��Oscene�A���ӭn����UI�h	
	/// </summary>
	class Playfield: public Container {


		int width, height;

		int load();

		// �~��playfield��class�A�bload���ɭԤ@�w�n�gŪ��map algo���u�@
		int load(FrameworkConfigManager* f);


	public:

		Playfield();

		//virtual int OnJudgement(Event* judge_event, Judgement* j);

		/// <summary>
		/// add events to EventProcessorMaster
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

		int GetWidth();

		int GetHeight();

	protected:

		///<summary>
		/// �b�o��s�F�|�Ψ쪺algo�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�algo
		///</summary>
		map<string, MapAlgorithmInterface*> mapAlgorithms;

		//Scheduler* scheduler;

		EventProcessorMaster* eventProcessorMaster;

		//Renderer* renderer;

		//Updater* updater;

		//Map* compositeGraph;

		///<summary>
		/// ���s��ا����v���A�٨S�첾�쥿�T��m�W
		///</summary>
		Map* bufferMap;
	};


}}}











#endif