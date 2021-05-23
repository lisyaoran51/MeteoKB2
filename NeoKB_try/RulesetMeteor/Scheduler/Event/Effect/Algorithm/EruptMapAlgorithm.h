#ifndef ERUPT_MAP_ALGORITHM_H
#define ERUPT_MAP_ALGORITHM_H




#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Games/Scheduler/Event/Effect/EffectMapper.h"
#include "../EruptEffect.h"
#include "../../../../Config/MeteorConfigManager.h"



using namespace Framework::Graphic::Maps;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Config;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class EruptMapAlgorithm: public MapAlgorithm<EruptEffect>
	{
		int load();

		int load(MeteorConfigManager * m);

	public:

		/// <summary>
		/// �L�\�Ϊ��غc�l�A�n�f�tlazy construct
		///	</summary>
		EruptMapAlgorithm();

	protected:

		int fallLength;

	};

	class EruptMapGenerateAlgorithm : public MapGenerateAlgorithm<EruptEffect> {

	public:

		EruptMapGenerateAlgorithm(MeteorConfigManager* m);

	protected:

		int fallLength;
		MTO_FLOAT fallBrightness;
		int whiteKeyTargetHeight;

		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<EruptEffect>* em);
	};
	

}}}}}












#endif