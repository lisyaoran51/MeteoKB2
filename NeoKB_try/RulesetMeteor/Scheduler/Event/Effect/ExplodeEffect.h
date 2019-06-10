#ifndef EXPLODE_EFFECT_H
#define EXPLODE_EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../../../../Base/Scheduler/Event/Effect/Effect.h"


using namespace std;
using namespace Util;
using namespace Base::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class ExplodeEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		ExplodeEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		ExplodeEffect(
			int xPos,
			int yPos,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT sp
		);

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	protected:
	};



	

}}}}



#endif