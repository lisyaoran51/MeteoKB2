#ifndef SHEETMUSIC_SELECT_PANEL_H
#define SHEETMUSIC_SELECT_PANEL_H

#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Sheetmusic/SheetmusicInfo.h"
#include "../Sheetmusic/SheetmusicManager.h"



using namespace Framework::Allocation::Hierachal;
using namespace Games::Sheetmusics;


namespace Games {
namespace UI {

	/// <summary>
	/// �b�o���h�q���A����ispresent���ӭn�令���U��song select��scene enter�Mresume�W�A���n�ۤv�h�]�wispresent
	/// </summary>
	class SheetmusicSelectPanel : public Container {

		

		int load();

		int load(FrameworkConfigManager* f, SmManager* s);

	public:

		SheetmusicSelectPanel();

		function<int(void)> StartRequest;

		function<int(SmInfo*)> SelectionChanged;

		int SetSms(vector<SmInfo*>* sInfos);


	protected:

		vector<SmInfo*>* smInfos = nullptr;

		SmManager* smManager;

		virtual int onCommand(InputState* inputState, InputKey command);

	};

}}




#endif