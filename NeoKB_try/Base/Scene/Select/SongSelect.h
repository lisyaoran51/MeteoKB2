#ifndef SONG_SELECT_H
#define SONG_SELECT_H


#include "../Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"


using namespace Base::Sheetmusics;


namespace Base {
namespace Scene {
namespace Select {

	/// <summary>
	/// 用update檢查歌曲有沒有被選取，如果被選取的話，就呼叫carouselSelectionChanged，
	/// 選擇完畢後，會將player loader 讀取進去，並且push player
	/// </summary>
	class SongSelect : public Scene {

	public:



	protected:

		/// <summary>
		/// 在選擇歌曲carouselSelectionChanged時執行，會把遊戲模式和譜綁在一起
		/// </summary>
		int updateSheetmusic(WorkingSm* workingSm);

	private:

		/// <summary>
		/// 讀取BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();


		/// <summary>
		/// 讀取
		/// </summary>
		//int load();

		/// <summary>
		/// 選擇歌曲後執行的程式
		/// </summary>
		int carouselSelectionChanged(SmInfo* sheetmusicInfo);
	};

}}}





#endif