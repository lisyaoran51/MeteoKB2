#ifndef MAIN_MENU_H
#define MAIN_MENU_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "SongSelect.h"
#include "../MeteoScene.h"
#include "../../UI/PowerPanel.h"
#include "../../UI/FirmwareUpgradePanel.h"


using namespace Games::Sheetmusics;
using namespace Framework::Scenes;
using namespace Games::UI;


namespace Games {
namespace Scenes {
namespace Select {

	/// <summary>
	/// ��update�ˬd�q�����S���Q����A�p�G�Q������ܡA�N�I�scarouselSelectionChanged�A
	/// ��ܧ�����A�|�Nplayer loader Ū���i�h�A�åBpush player
	/// </summary>
	class MainMenu : public MeteoScene {

	public:

		MainMenu();

	protected:

		Instrument* instrument = nullptr;

		SongSelect* songSelect = nullptr;

		PowerPanel* powerPanel = nullptr;

		FirmwareUpgradePanel* firmwareUpgradePanel = nullptr;

		virtual int onEntering(Scene* lastScene);

	private:

		/// <summary>
		/// Ū��BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();

		int load(Instrument* i);


		/// <summary>
		/// Ū��
		/// </summary>
		//int load();

	};

}}}





#endif