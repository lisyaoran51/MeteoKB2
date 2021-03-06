#ifndef GLOWLINE_MAP_ALGORITHM_H
#define GLOWLINE_MAP_ALGORITHM_H


#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../GlowLineEffect.h"
#include "../GlowLineEffectMapper.h"


using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class GlowLineMapAlgorithm: public MapAlgorithm<GlowLineEffect>
	{

		int load();

	public:

		/// <summary>
		/// 無功用的建構子，要搭配lazy construct
		///	</summary>
		GlowLineMapAlgorithm();

	};

	class GlowLineMapGenerateAlgorithm : public MapGenerateAlgorithm<GlowLineEffect> {


	public:

	protected:
		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<GlowLineEffect>* em);
	};

	

}}}}}








#endif