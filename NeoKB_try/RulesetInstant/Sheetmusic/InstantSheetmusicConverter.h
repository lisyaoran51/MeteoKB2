#ifndef INSTANT_SHEETMUSIC_CONVERTER_H
#define INSTANT_SHEETMUSIC_CONVERTER_H

#include "../../Games/Sheetmusic/SheetmusicConverter.h"

using namespace Games::Sheetmusics;


namespace Instant {
namespace Sheetmusics{

	class InstantSmConverter : public SmConverter {

	public:
		/// <summary>
		/// 把設定讀進來
		///	</summary>
		InstantSmConverter(PatternGenerator* pg);

		/// <summary>
		/// converts the events in sm
		/// We always operate on a clone of the original sm, to not modify it game-wide
		/// 另外加入額外特效，這些特效是專屬於meteor的
		///	</summary>
		virtual Sm<Event>* Convert(Sm<Event>* s);


	protected:

		int convertEvent(vector<Event*>* es, Event* e);

	};


}
}






#endif