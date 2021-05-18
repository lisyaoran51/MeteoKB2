#include "MainMenu.h"

#include "PlaySongSelect.h"
#include "../../Output/Panels/IndicatorLightPanelMessage.h"
#include "../../Output/Panels/LightRingPanelMessage.h"


using namespace Games::Scenes::Select;
using namespace Games::Output::Panels;


MainMenu::MainMenu(): RegisterType("MainMenu")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(MainMenu::*)())&MainMenu::load, this));
}

int MainMenu::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	songSelect = new PlaySongSelect();

	Push(songSelect);
	//GetScheduler()->AddDelayedTask([=]() { Push(songSelect); return 0; }, 0.5);

	return 0;
}

int MainMenu::load()
{
	LOG(LogLevel::Info) << "MainMenu::load() : Start loading main menu.";

	Instrument* i = GetCache<Instrument>("Instrument");

	if (!i)
		throw runtime_error("int MainMenu::load() : Instrument not found in cache.");
	return load(i);
}

int MainMenu::load(Instrument * i)
{
	instrument = i;

	powerPanel = new PowerPanel();

	powerPanel->PowerOffRequest = [=]() {
		Resume(songSelect);
		songSelect = nullptr;

		instrument->Sleep();

		// TODO: 關掉panel所有燈光

		IndicatorLightPanelMessage* indicatorLightMessage = new IndicatorLightPanelMessage(-1, false);
		
		outputManager->PushMessage(indicatorLightMessage);

		LightRingPanelMessage* lightRingOutputMessage = new LightRingPanelMessage(-1);

		outputManager->PushMessage(lightRingOutputMessage);

		return 0;
	};

	powerPanel->PowerOnRequest = [=]() {
		instrument->WakeUp();


		LOG(LogLevel::Depricated) << "MainMenu::PowerOnRequest() : delete working sm.";

		if (workingSm.GetValue() != nullptr) {
			delete workingSm.GetValue();
			workingSm.SetValue(nullptr);
		}

		LOG(LogLevel::Depricated) << "MainMenu::PowerOnRequest() : delete ruleset info.";

		if (rulesetInfo.GetValue() != nullptr) {
			rulesetInfo.SetValue(nullptr);
		}

		LOG(LogLevel::Depricated) << "MainMenu::PowerOnRequest() : push songselect.";
		
		songSelect = new PlaySongSelect();

		Push(songSelect);

		LOG(LogLevel::Depricated) << "MainMenu::PowerOnRequest() : push songselect over.";

		// TODO: 打開panel燈光

		IndicatorLightPanelMessage* indicatorLightMessage = new IndicatorLightPanelMessage(0, true);

		outputManager->PushMessage(indicatorLightMessage);

		return 0;
	};

	AddChild(powerPanel);

	return 0;
}
