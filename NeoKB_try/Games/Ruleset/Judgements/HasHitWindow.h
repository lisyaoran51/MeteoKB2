#ifndef I_HAS_HIT_WINDOW_H
#define I_HAS_HIT_WINDOW_H


#include "HitWindow.h"




namespace Games {
namespace Rulesets{
namespace Judgements{

	class HasHitWindow {




	public:

		/// <summary>
		///	�o�ӥΨӳ]�w�C�����ת��ƭȡA��Jhit window��
		/// </summary>
		// virtual int ApplyDefault(ControlePointInfo*, SmDifficulty*);

		virtual HitResult GetResultFor(double offset);

		virtual double GetHitWindow(HitResult window);


	protected:

		HitWindow* hitWindow = nullptr;

	};


}}}






#endif
