#ifndef SHEETMUSIC_POSTPROCESSOR_H
#define SHEETMUSIC_POSTPROCESSOR_H


#include "Sheetmusic.h"
#include "../Scheduler/Event/Event.h"


using namespace std;
using namespace Util;
using namespace Base::Schedulers::Events;
using namespace Base::Sheetmusics;



namespace Base {
namespace Sheetmusics {

	/// <summary>
	/// 在遊戲load進歌曲並且轉換以後，這個可以做一些後處理，例如說把Event重新排序，或把重疊的effect修掉
	///	</summary>
	class SmPostprocessor
	{

	public:

		/// <summary>
		/// 在遊戲load進歌曲並且轉換以後，這個可以做一些後處理
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);


	};


}
}





#endif