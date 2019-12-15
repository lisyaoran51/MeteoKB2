#ifndef HIT_WINDOW_H
#define HIT_WINDOW_H

#include "HitResult.h"

namespace Games {
namespace Rulesets{
namespace Judgements{


	/// <summary>
	///	hit window是osu的命名，他功能是給判斷分數的一個標準，例如時間多接近算是good
	///
	/// </summary>
	class HitWindow {

	public:

		virtual HitResult GetResultFor(double hitOffset) = 0;

		virtual double GetHitWindow(HitResult hResult) = 0;
	};

}}}






#endif
