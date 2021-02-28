#ifndef RECORD_SHEETMUSIC_CONVERTER_H
#define RECORD_SHEETMUSIC_CONVERTER_H

#include "../../Games/Sheetmusic/SheetmusicConverter.h"

using namespace Games::Sheetmusics;


namespace Record {
namespace Sheetmusics{

	class RecordSmConverter : public SmConverter {

	public:
		/// <summary>
		/// ��]�wŪ�i��
		///	</summary>
		RecordSmConverter(PatternGenerator* pg);

		/// <summary>
		/// converts the events in sm
		/// We always operate on a clone of the original sm, to not modify it game-wide
		/// �t�~�[�J�B�~�S�ġA�o�ǯS�ĬO�M�ݩ�meteor��
		///	</summary>
		virtual Sm<Event>* Convert(Sm<Event>* s);


	protected:

		int convertEvent(vector<Event*>* es, Event* e);

	};


}
}






#endif