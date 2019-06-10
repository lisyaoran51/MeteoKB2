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
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
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

		// 一定要每次都override!!
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