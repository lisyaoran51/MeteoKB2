#ifndef INSTANT_GLOW_LINE_EFFECT_H
#define INSTANT_GLOW_LINE_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;


namespace Instant {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class InstantGlowLineEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		InstantGlowLineEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		InstantGlowLineEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l
		);

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	protected:

	};



	

}}}}



#endif