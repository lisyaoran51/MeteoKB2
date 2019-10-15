#ifndef TARGETLINE_EFFECT_H
#define TARGETLINE_EFFECT_H


#include <string>
#include <fstream>
#include <vector>
#include "../../../../Games/Scheduler/Event/Effect/Effect.h"


using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events::Effects;


namespace Meteor {
namespace Schedulers {
namespace Events {
namespace Effects {
	
	/// <summary>
	/// �����u�S�ġA�|�b�^��W�{�{�A�������ӭn�Mglow line effect 
	/// </summary>
	class TargetLineEffect: public Effect
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		TargetLineEffect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		TargetLineEffect(
			int wHeight,
			int bHeight,
			MTO_FLOAT sTime,
			MTO_FLOAT l,
			MTO_FLOAT bSpeed
		);

		/* �@�w�n�C����override!! */
		virtual string GetTypeName();

	protected:

		int whiteKeyTargetHeight;
		int blackKeyTargetHeight;

	};



	

}}}}





#endif