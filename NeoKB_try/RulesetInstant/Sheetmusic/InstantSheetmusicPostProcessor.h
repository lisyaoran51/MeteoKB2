#ifndef INSTANT_SHEETMUSIC_POSTPROCESSOR_H
#define INSTANT_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/InstantConfigManager.h"

using namespace Instant::Config;

namespace Instant {
namespace Sheetmusics {

	/// <summary>
	/// ��C���[�J��L�S�ĩM�����u
	///	</summary>
	class InstantSmPostprocessor: public SmPostprocessor
	{


	public:

		InstantSmPostprocessor();

		/// <summary>
		/// �b�C��load�i�q���åB�ഫ�H��A�o�ӥi�H���@�ǫ�B�z
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

	};


}}





#endif
