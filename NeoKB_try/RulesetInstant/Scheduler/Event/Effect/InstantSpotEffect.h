#ifndef INSTANT_SPOT_EFFECT_H
#define INSTANT_SPOT_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../Games/Scheduler/Event/Effect/EffectPinType.h"
#include "../../../../Instruments/Pitch.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Instruments;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class InstantSpotEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantSpotEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantSpotEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantSpotEffect(
			Pitch p,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		EffectPinType GetEffectPinType();

		Pitch GetPitch();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		virtual Effect* Clone();

	protected:

		EffectPinType effectPinType = EffectPinType::ByPitch;

		Pitch pitch = Pitch::None;

	};



	

}}}}



#endif