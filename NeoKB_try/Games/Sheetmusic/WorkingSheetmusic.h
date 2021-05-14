#ifndef WORKING_SHEETMUSIC_H
#define WORKING_SHEETMUSIC_H

#include <string>
#include <fstream>
#include <vector>
#include "../../Util/TemplateConstraint.h"
#include "Sheetmusic.h"
#include "SheetmusicInfo.h"
#include "Format/SmDecoder.h"
#include "../Ruleset/Modifiers/Modifier.h"
#include "../../Util/DataStructure/BindablePointer.h"
#include "../../Framework/Audio/Track/Track.h"



using namespace std;
using namespace Util::DataStructure;
using namespace Games::Schedulers::Events;
using namespace Games::Rulesets::Modifiers;
using namespace Framework::Audio::Tracks;


namespace Games {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class WorkingSm
	{

	public:

		WorkingSm(SmInfo* s);

		virtual ~WorkingSm();

		virtual Sm<Event>* GetSm();

		bool IsTheSameSm(SmInfo* s);

		BindablePointer<vector<Modifier*>*>* GetModifiers();

		virtual Track* GetTrack();

	protected:


		// 這個直接拿sm info裡的舊好
		//SmSetInfo* smSetInfo = NULL;

		SmInfo* smInfo = nullptr;

		SmMetadata* smMetadata = nullptr;

		//vector<ctrl_point>* effects;

		BindablePointer<vector<Modifier*>*> modifiers;

		Sm<Event>* sm = nullptr;

		virtual Sm<Event>* createSm();

		virtual string getPathForFile(string fileName);

		Track* track = nullptr;

	};


}
}



#endif