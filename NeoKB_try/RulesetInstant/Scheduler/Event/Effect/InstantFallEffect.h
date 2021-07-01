#ifndef INSTANT_FALL_EFFECT_H
#define INSTANT_FALL_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../Games/Scheduler/Event/Effect/EffectPinType.h"
#include "../../../../Instruments/Pitch.h"
#include "../../../../Instruments/HasPitch.h"
#include "../../../../Games/Ruleset/Judgements/HasHitWindow.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Rulesets::Judgements;
using namespace Instruments;



namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
	
	

	class InstantFallEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantFallEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantFallEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT sp
		);

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantFallEffect(
			Pitch p,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT sp
		);

		virtual int ApplyDefaultValues(SmDifficulty* smDifficulty);

		int SetTargetHeight(int tHeight);

		/// <summary>
		/// �����n���]SetTargetHeight�A����~�i�Hadjust speed�A���M�|�����~�A�]���S��target height�A�|�L�k����}�l�ɶ�
		///	</summary>
		int AdjustSpeed(float sp);

		EffectPinType GetEffectPinType();

		Pitch GetPitch();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		virtual Effect* Clone();


	protected:

		int targetHeight = -1;

		bool isSpeedAdjustable = false;

		EffectPinType effectPinType = EffectPinType::ByPitch;

		Pitch pitch = Pitch::None;


	};



	

}}}}



#endif