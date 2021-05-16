#include "MainMenu.h"

#include "PlaySongSelect.h"


using namespace Games::Scenes::Select;


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


		return 0;
	};

	powerPanel->PowerOnRequest = [=]() {
		instrument->WakeUp();

		if (workingSm.GetValue() != nullptr) {
			delete workingSm.GetValue();
			workingSm.SetValue(nullptr);
		}

		if (rulesetInfo.GetValue() != nullptr) {
			delete rulesetInfo.GetValue();
			rulesetInfo.SetValue(nullptr);
		}
		
		songSelect = new PlaySongSelect();

		Push(songSelect);

		LOG(LogLevel::Info) << "MainMenu::PowerOnRequest() : push songselect over.";

		return 0;
	};

	AddChild(powerPanel);

	return 0;
}
