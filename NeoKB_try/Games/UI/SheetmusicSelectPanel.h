#ifndef SHEETMUSIC_SELECT_PANEL_H
#define SHEETMUSIC_SELECT_PANEL_H

#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Sheetmusic/SheetmusicInfo.h"
#include "../Sheetmusic/SheetmusicManager.h"
#include "../../Framework/Input/Commands/CommandHandler.h"
#include "../Input/Commands/MeteoBluetoothCommand.h"



using namespace Framework::Allocation::Hierachal;
using namespace Games::Sheetmusics;
using namespace Framework::Input::Commands;
using namespace Games::Input::Commands;


namespace Games {
namespace UI {

	/// <summary>
	/// �b�o���h�q���A����ispresent���ӭn�令���U��song select��scene enter�Mresume�W�A���n�ۤv�h�]�wispresent
	/// </summary>
	class SheetmusicSelectPanel : public Container, public CommandHandler<MeteoBluetoothCommand> {

		

		int load();

		int load(FrameworkConfigManager* f, SmManager* s, OutputManager* o);

	public:

		SheetmusicSelectPanel();

		BindablePointer<vector<Modifier*>*>* GetSelectedModifiers();

		int AddModifier(Modifier* modifier);

		function<int(void)> StartRequest;

		function<int(SmInfo*)> SelectionChanged;

		int SetSms(vector<SmInfo*>* sInfos);

		virtual int OnCommand(MeteoBluetoothCommand* command);

	protected:

		vector<SmInfo*>* smInfos = nullptr;

		SmManager* smManager = nullptr;

		FrameworkConfigManager* frameworkConfigManager = nullptr;

		OutputManager* outputManager = nullptr;

		BindablePointer<vector<Modifier*>*> selectedModifiers;

		virtual int onCommand(InputState* inputState, InputKey command);

	};

}}




#endif