#ifndef JUDGEMENT_H
#define JUDGEMENT_H


#include "HitResult.h"
#include <string>


using namespace std;


namespace Games {
namespace Rulesets{
namespace Judgements{

	class Judgement {

	public:

		Judgement(HitResult r);

		HitResult GetResult();

		double GetResultScore();

		string GetResultString();

		double GetMaxResultScore();

		int SetTimeOffset(double offset);

		bool GetIsHit();

	protected:

		double timeOffset = 0.0;

		HitResult hitResult = HitResult::None;

		virtual double getResultNumber(HitResult hResult) = 0;

	};

}
}
}





#endif