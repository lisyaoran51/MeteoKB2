#ifndef HIT_WINDOW_H
#define HIT_WINDOW_H

#include "HitResult.h"
#include "../../Sheetmusic/SheetmusicDifficulty.h"



using namespace Games::Sheetmusics;

namespace Games {
namespace Rulesets{
namespace Judgements{


	/// <summary>
	///	hit window�Oosu���R�W�A�L�\��O���P�_���ƪ��@�ӼзǡA�Ҧp�ɶ��h�����Ogood
	///
	/// </summary>
	class HitWindow {

	public:

		HitWindow(SmDifficulty* smDifficulty = nullptr);

		virtual HitResult GetResultFor(double hitOffset) = 0;

		virtual double GetHitWindow(HitResult hResult) = 0;
	};

}}}






#endif
