#ifndef INSTANT_SPOT_MAP_ALGORITHM_H
#define INSTANT_SPOT_MAP_ALGORITHM_H


#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../InstantSpotEffect.h"
#include "../InstantSpotEffectMapper.h"


using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Instant::Schedulers::Events::Effects;



namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class InstantSpotMapAlgorithm: public MapAlgorithm<InstantSpotEffect>
	{

		int load();

	public:

		/// <summary>
		/// �L�\�Ϊ��غc�l�A�n�f�tlazy construct
		///	</summary>
		InstantSpotMapAlgorithm();

	};

	class InstantSpotMapGenerateAlgorithm : public MapGenerateAlgorithm<InstantSpotEffect> {


	public:

	protected:
		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<InstantSpotEffect>* em);
	};

	

}}}}}








#endif