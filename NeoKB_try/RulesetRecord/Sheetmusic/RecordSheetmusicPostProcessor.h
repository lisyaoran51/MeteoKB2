#ifndef RECORD_SHEETMUSIC_POSTPROCESSOR_H
#define RECORD_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/RecordConfigManager.h"

using namespace Record::Config;

namespace Record {
namespace Sheetmusics {

	/// <summary>
	/// ��C���[�J��L�S�ĩM�����u
	///	</summary>
	class RecordSmPostprocessor: public SmPostprocessor
	{


	public:

		RecordSmPostprocessor();

		/// <summary>
		/// �b�C��load�i�q���åB�ഫ�H��A�o�ӥi�H���@�ǫ�B�z
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

	};


}}





#endif
