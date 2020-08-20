#ifndef INSTANT_SHEETMUSIC_POSTPROCESSOR_H
#define INSTANT_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/InstantConfigManager.h"

using namespace Instant::Config;

namespace Instant {
namespace Sheetmusics {

	/// <summary>
	/// 位遊戲加入其他特效和打擊線
	///	</summary>
	class InstantSmPostprocessor: public SmPostprocessor
	{


	public:

		InstantSmPostprocessor();

		/// <summary>
		/// 在遊戲load進歌曲並且轉換以後，這個可以做一些後處理
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

	};


}}





#endif
