#ifndef METEOR_SHEETMUSIC_POSTPROCESSOR_H
#define METEOR_SHEETMUSIC_POSTPROCESSOR_H


#include "../../Games/Sheetmusic/SheetmusicPostprocessor.h"
#include "../Config/MeteorConfigManager.h"

using namespace Meteor::Config;

namespace Meteor {
namespace Sheetmusics {

	/// <summary>
	/// ��C���[�J��L�S�ĩM�����u
	///	</summary>
	class MeteorSmPostprocessor: public SmPostprocessor
	{


	public:

		MeteorSmPostprocessor();

		/// <summary>
		/// �b�C��load�i�q���åB�ഫ�H��A�o�ӥi�H���@�ǫ�B�z
		///	</summary>
		virtual Sm<Event>* postprocess(Sm<Event>* s);

	protected:

		MTO_FLOAT pedalInterval = 0.2f;
		MTO_FLOAT defaultNextPedalTargetStartTime = 1.f;

	};


}}





#endif
