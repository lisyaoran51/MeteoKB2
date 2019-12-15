#ifndef HIT_WINDOW_H
#define HIT_WINDOW_H

#include "HitResult.h"

namespace Games {
namespace Rulesets{
namespace Judgements{


	/// <summary>
	///	hit window�Oosu���R�W�A�L�\��O���P�_���ƪ��@�ӼзǡA�Ҧp�ɶ��h�����Ogood
	///
	/// </summary>
	class HitWindow {

	public:

		virtual HitResult GetResultFor(double hitOffset) = 0;

		virtual double GetHitWindow(HitResult hResult) = 0;
	};

}}}






#endif
