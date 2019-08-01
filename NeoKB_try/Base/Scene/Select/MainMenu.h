#ifndef MAIN_MENU_H
#define MAIN_MENU_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"


using namespace Base::Sheetmusics;
using namespace Framework::Scene;


namespace Base {
namespace Scene {
namespace Select {

	/// <summary>
	/// 用update檢查歌曲有沒有被選取，如果被選取的話，就呼叫carouselSelectionChanged，
	/// 選擇完畢後，會將player loader 讀取進去，並且push player
	/// </summary>
	class MainMenu : public Scene {

	public:



	protected:

		virtual int onEntering(Scene* lastScene);

	private:

		/// <summary>
		/// 讀取BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();


		/// <summary>
		/// 讀取
		/// </summary>
		//int load();

	};

}}}





#endif