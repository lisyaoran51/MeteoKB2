#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include <fstream>
#include <vector>
#include "../Event.h"


using namespace std;
using namespace Util;
using namespace Base::Schedulers::Events;


namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {
	

	class Effect: public Event
	{

	public:

		/// <summary>
		/// �o�ӫغc�l���O�ΨӰ��檺�A���ܥΨӵ�TConstraint�ӽT�{�~�����Y��
		///	</summary>
		Effect();

		/// <summary>
		/// construct an immediate effect
		///	</summary>
		Effect(
			int xPos,
			int yPos,
			MTO_FLOAT start_t,
			MTO_FLOAT life_t
		);

		Effect(const Effect&);

		// �@�w�n�C����override!!
		virtual string GetTypeName();
		int GetX();
		int GetY();
		MTO_FLOAT GetSpeed();

		int SetBrightness(MTO_FLOAT b);
		int SetSpeed(MTO_FLOAT s);

		virtual Effect* Clone();

	protected:

		int x;
		int y;
		MTO_FLOAT brightness;	// default as 1.0
		MTO_FLOAT speed;		// default as 1.0

		Effect(const Effect*);
	};



	

}}}}



#endif