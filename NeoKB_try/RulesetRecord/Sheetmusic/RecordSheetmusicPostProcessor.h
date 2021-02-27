#ifndef RECORD_SHEETMUSIC_POSTPROCESSOR_H
#define RECORD_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/RecordConfigManager.h"

using namespace Record::Config;

namespace Record {
namespace Sheetmusics {

	/// <summary>
	/// 位遊戲加入其他特效和打擊線
	///	</summary>
	class RecordSmPostprocessor: public SmPostprocessor
	{


	public:

		RecordSmPostprocessor();

		/// <summary>
		/// 在遊戲load進歌曲並且轉換以後，這個可以做一些後處理
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

	};


}}





#endif
