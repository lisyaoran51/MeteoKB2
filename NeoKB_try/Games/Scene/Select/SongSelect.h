#ifndef SONG_SELECT_H
#define SONG_SELECT_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../Sheetmusic/SheetmusicManager.h"
#include "../MeteoScene.h"
#include "../../../Games/UI/SheetmusicSelectPanel.h"
#include "../../UI/SoundSelectPanel.h"
//#include "../../MeteoGame.h" // ���i�Hinclude ��recursive


using namespace Games::Sheetmusics;
using namespace Framework::Scenes;
using namespace Util::DataStructure;
using namespace Games::Scenes;
using namespace Games::UI;
using namespace Games;


namespace Games {
namespace Scenes {
namespace Select {

	/// <summary>
	/// ��update�ˬd�q�����S���Q����A�p�G�Q������ܡA�N�I�scarouselSelectionChanged�A
	/// ��ܧ�����A�|�Nplayer loader Ū���i�h�A�åBpush player
	/// </summary>
	class SongSelect : public MeteoScene {

	public:

		SongSelect();

		/// <summary>
		/// ��ܧ����A�i�J�U�ӳ���
		/// </summary>
		virtual int TriggerOnSelected();

	protected:

		SmManager* smManager = nullptr;

		SheetmusicSelectPanel* smSelectPanel = nullptr;

		SoundSelectPanel* soundSelectPanel = nullptr;

		/// <summary>
		/// �b��ܺq��carouselSelectionChanged�ɰ���A�|��C���Ҧ��M�иj�b�@�_
		/// </summary>
		virtual int updateSheetmusic(WorkingSm* workingSm);
		
		/// <summary>
		/// ��ܧ����A�i�J�U�ӳ���
		/// </summary>
		virtual int onSelected() = 0;

	private:

		/// <summary>
		/// Ū��BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();



		/// <summary>
		/// Ū��
		/// </summary>
		int load(SmManager* sManager, MeteoGame* game);

		/// <summary>
		/// ��ܺq������檺�{��
		/// </summary>
		int selectionChanged(SmInfo* sheetmusicInfo);
	};

}}}





#endif