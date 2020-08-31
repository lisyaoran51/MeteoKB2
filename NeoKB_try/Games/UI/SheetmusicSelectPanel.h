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
	/// 在這邊選則歌曲，之後ispresent應該要改成註冊到song select的scene enter和resume上，不要自己去設定ispresent
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