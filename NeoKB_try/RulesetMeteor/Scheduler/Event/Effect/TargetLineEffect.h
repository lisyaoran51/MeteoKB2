#ifndef TARGETLINE_EFFECT_H
#define TARGETLINE_EFFECT_H


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
	
	/// <summary>
	/// 打擊線特效，會在琴鍵上閃爍，之後應該要和glow line effect 
	/// </summary>
	class TargetLineEffect: public Effect
	{

	public:

		/// <summary>
		/// 這個建構子不是用來執行的，指示用來給TConstraint來確認繼承關係用
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

		/* 一定要每次都override!! */
		virtual string GetTypeName();

	protected:

		int whiteKeyTargetHeight;
		int blackKeyTargetHeight;

	};



	

}}}}





#endif