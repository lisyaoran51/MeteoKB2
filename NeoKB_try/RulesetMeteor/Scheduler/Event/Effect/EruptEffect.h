#ifndef ERUPT_EFFECT_H
#define ERUPT_EFFECT_H


#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"
#include "../../../../Instruments/Pitch.h"
#include "../../../../Instruments/HasPitch.h"
#include "../../../../Games/Ruleset/Judgements/HasHitWindow.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;
using namespace Games::Rulesets::Judgements;
using namespace Instruments;



namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class EruptEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		EruptEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		EruptEffect(
			int xPos,
			int yPos,
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

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	protected:

		int targetHeight = -1;

		bool isSpeedAdjustable = false;
	};



	

}}}}



#endif