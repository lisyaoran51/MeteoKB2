#ifndef METEOR_SHEETMUSIC_POSTPROCESSOR_H
#define METEOR_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/MeteorConfigManager.h"

using namespace Meteor::Config;

namespace Meteor {
namespace Sheetmusics {

	/// <summary>
	/// 位遊戲加入其他特效和打擊線
	///	</summary>
	class MeteorSmPostprocessor: public SmPostprocessor
	{


	public:

		MeteorSmPostprocessor();

		/// <summary>
		/// 在遊戲load進歌曲並且轉換以後，這個可以做一些後處理
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

		MTO_FLOAT pedalInterval = 0.2f;
		MTO_FLOAT defaultNextPedalTargetStartTime = 1.f;

	};


}}





#endif
