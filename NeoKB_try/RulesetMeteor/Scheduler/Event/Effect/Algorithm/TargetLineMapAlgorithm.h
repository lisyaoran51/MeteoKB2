#ifndef TARGETLINE_MAP_ALGORITHM_H
#define TARGETLINE_MAP_ALGORITHM_H

#include"../../../../../Base/Graphic/Map/Map.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapGenerateAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../../../Base/Scheduler/Event/Effect/Effect.h"
#include "../../../../../Base/Scheduler/Event/Effect/EffectMapper.h"
#include "../TargetLineEffect.h"
#include "../../../../Config/MeteorConfigManager.h"



using namespace Base::Graphic::Maps;
using namespace Base::Schedulers::Events::Effects::Algorithms;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::Effects;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Meteor::Config;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	


	class TargetLineMapAlgorithm: public MapAlgorithm<TargetLineEffect>
	{
		int load();

		int load(MeteorConfigManager * m);

	public:

		/// <summary>
		/// �L�\�Ϊ��غc�l�A�n�f�tlazy construct
		///	</summary>
		TargetLineMapAlgorithm();

	protected:

		MTO_FLOAT blinkSpeed;

	};

	class TargetLineMapGenerateAlgorithm : public MapGenerateAlgorithm<TargetLineEffect> {

		bool isWhiteKey(int key);

	public:

		TargetLineMapGenerateAlgorithm(MeteorConfigManager* m);

	protected:

		MTO_FLOAT blinkSpeed;			// �C��{�{����
		MTO_FLOAT blinkInterval;		// �C���{�{���ɶ��A���P��1/blinkSpeed
		MTO_FLOAT targetLineBrightness;

		int whiteKeyTargetLineHeight;
		int blackKeyTargetLineHeight;

		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<TargetLineEffect>* em);
	};
	

}}}}}


#endif