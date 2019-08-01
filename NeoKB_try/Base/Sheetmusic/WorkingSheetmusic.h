#ifndef WORKING_SHEETMUSIC_H
#define WORKING_SHEETMUSIC_H

#include <string>
#include <fstream>
#include <vector>
#include "../../Util/TemplateConstraint.h"
#include "Sheetmusic.h"
#include "SheetmusicInfo.h"
#include "Format/SmDecoder.h"



using namespace std;
using namespace Util;
using namespace Base::Schedulers::Events;

namespace Base {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class WorkingSm
	{
		// �o�Ӫ�����sm info�̪��¦n
		//SmSetInfo* smSetInfo = NULL;

		SmInfo* smInfo = NULL;

		SmMetadata* smMetadata = NULL;

		//vector<ctrl_point>* effects;

		//vector<Mod*> mods;

		Sm<Event>* sm = NULL;

	public:

		WorkingSm(SmInfo* s);

		~WorkingSm();

		Sm<Event>* GetSm();

		bool IsTheSameSm(SmInfo* s);

		Bindable<vector<Modifier*>*>* GetModifiers();

	protected:

		virtual Sm<Event>* createSm();

		string getPathForFile(string fileName);

	};


}
}



#endif