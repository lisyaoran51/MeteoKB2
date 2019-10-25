#ifndef SONG_SELECT_H
#define SONG_SELECT_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../../Util/DataStructure/Bindable.h"
#include "../../Sheetmusic/SheetmusicManager.h"
#include "../MeteoScene.h"
#include "../../../Games/UI/SheetmusicSelectPanel.h"


using namespace Games::Sheetmusics;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Games::Scenes;
using namespace Games::UI;


namespace Games {
namespace Scenes {
namespace Select {

	/// <summary>
	/// 用update檢查歌曲有沒有被選取，如果被選取的話，就呼叫carouselSelectionChanged，
	/// 選擇完畢後，會將player loader 讀取進去，並且push player
	/// </summary>
	class SongSelect : public MeteoScene {

		SheetmusicSelectPanel* smSelectPanel;

	public:

		SongSelect();

		/// <summary>
		/// 選擇完成，進入下個場景
		/// </summary>
		virtual int TriggerOnSelected();

	protected:

		SmManager* smManager;

		/// <summary>
		/// 在選擇歌曲carouselSelectionChanged時執行，會把遊戲模式和譜綁在一起
		/// </summary>
		virtual int updateSheetmusic(WorkingSm* workingSm);
		
		/// <summary>
		/// 選擇完成，進入下個場景
		/// </summary>
		virtual int onSelected() = 0;

	private:

		/// <summary>
		/// 讀取BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();



		/// <summary>
		/// 讀取
		/// </summary>
		int load(SmManager* sManager, MeteoGame* game);

		/// <summary>
		/// 選擇歌曲後執行的程式
		/// </summary>
		int selectionChanged(SmInfo* sheetmusicInfo);
	};

}}}





#endif