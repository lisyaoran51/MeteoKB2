#ifndef SHEETMUSIC_SELECT_PANEL_H
#define SHEETMUSIC_SELECT_PANEL_H

#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../Sheetmusic/SheetmusicInfo.h"
#include "../Sheetmusic/SheetmusicManager.h"
#include "../../Framework/Input/Messages/MessageHandler.h"
#include "../Output/Bluetooths/MeteoBluetoothMessage.h"
#include "../../Framework/IO/Communications/CommunicationAccess.h"
#include "../../Util/DataStructure/FileSegmentMap.h"
#include <functional>



using namespace Framework::Allocation::Hierachal;
using namespace Games::Sheetmusics;
using namespace Framework::Input::Messages;
using namespace Games::Output::Bluetooths;
using namespace Framework::IO::Communications;
using namespace Util::DataStructure;
using namespace std;


namespace Games {
namespace UI {

	/// <summary>
	/// 在這邊選則歌曲，之後ispresent應該要改成註冊到song select的scene enter和resume上，不要自己去設定ispresent
	/// </summary>
	class SheetmusicSelectPanel : public Container, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(FrameworkConfigManager* f, SmManager* s, OutputManager* o, CommunicationAccess* c);

	public:

		SheetmusicSelectPanel();

		BindablePointer<vector<Modifier*>*>* GetSelectedModifiers();

		int AddModifier(Modifier* modifier);

		function<int(void)> StartRequest;

		function<int(SmInfo*)> SelectionChanged;

		int SetSms(vector<SmInfo*>* sInfos);

		int SelectAndStart(string fileName);

		int AddOnDownloadSheetmusicSuccess(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleDownloadSheetmusicSuccess");

		int AddOnDownloadSheetmusicFail(MtoObject * callableObject, function<int(FileSegmentMap*)> callback, string name = "HandleDownloadSheetmusicFail");


	protected:

		vector<SmInfo*>* smInfos = nullptr;

		SmManager* smManager = nullptr;

		FrameworkConfigManager* frameworkConfigManager = nullptr;

		OutputManager* outputManager = nullptr;

		CommunicationAccess* communicationAccess = nullptr;

		BindablePointer<vector<Modifier*>*> selectedModifiers;

		ActionList<int, FileSegmentMap*> onGetSheetmusicSuccess;

		ActionList<int, FileSegmentMap*> onGetSheetmusicFail;

		virtual int onMessage(InputState* inputState, InputKey command);

		virtual int onMessage(MeteoBluetoothMessage* message);
	};

}}




#endif