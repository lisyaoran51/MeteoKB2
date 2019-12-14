#ifndef I_HAS_HIT_WINDOW_H
#define I_HAS_HIT_WINDOW_H


#include "HitWindow.h"




namespace Games {
namespace Rulesets{
namespace Judgements{

	class HasHitWindow {




	public:

		/// <summary>
		///	這個用來設定每個難度的數值，放入hit window中
		/// </summary>
		// virtual int ApplyDefault(ControlePointInfo*, SmDifficulty*);

		virtual HitResult GetResultFor(double offset);

		virtual double GetHitWindow(HitResult window);


	protected:

		HitWindow* hitWindow = nullptr;

	};


}}}






#endif
