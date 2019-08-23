#ifndef PLAY_SONG_SELECT_H
#define PLAY_SONG_SELECT_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../../Util/DataStructure/Bindable.h"
#include "../../Sheetmusic/SheetmusicManager.h"
#include "SongSelect.h"
#include "../../Ruleset/Modifiers/Modifier.h"


using namespace Base::Sheetmusics;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Base::Rulesets::Modifiers;


namespace Base {
namespace Scenes {
namespace Select {

	/// <summary>
	/// 用update檢查歌曲有沒有被選取，如果被選取的話，就呼叫carouselSelectionChanged，
	/// 選擇完畢後，會將player loader 讀取進去，並且push player
	/// </summary>
	class PlaySongSelect : public SongSelect {

		vector<Modifier*> selectedModifiers;

		Scene* player;

	public:

		PlaySongSelect();

	protected:

		/// <summary>
		/// 在選擇歌曲carouselSelectionChanged時執行，會把遊戲模式和譜綁在一起
		/// </summary>
		virtual int updateSheetmusic(WorkingSm* workingSm);

		/// <summary>
		/// 選擇完成，進入下個場景
		/// </summary>
		virtual int onSelected();

	private:

		/// <summary>
		/// 讀取BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();
	};

}}}





#endif