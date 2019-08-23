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
	/// ��update�ˬd�q�����S���Q����A�p�G�Q������ܡA�N�I�scarouselSelectionChanged�A
	/// ��ܧ�����A�|�Nplayer loader Ū���i�h�A�åBpush player
	/// </summary>
	class PlaySongSelect : public SongSelect {

		vector<Modifier*> selectedModifiers;

		Scene* player;

	public:

		PlaySongSelect();

	protected:

		/// <summary>
		/// �b��ܺq��carouselSelectionChanged�ɰ���A�|��C���Ҧ��M�иj�b�@�_
		/// </summary>
		virtual int updateSheetmusic(WorkingSm* workingSm);

		/// <summary>
		/// ��ܧ����A�i�J�U�ӳ���
		/// </summary>
		virtual int onSelected();

	private:

		/// <summary>
		/// Ū��BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();
	};

}}}





#endif