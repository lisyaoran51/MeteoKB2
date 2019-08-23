#ifndef SONG_SELECT_H
#define SONG_SELECT_H


#include "../../../Framework/Scenes/Scene.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../../Sheetmusic/WorkingSheetmusic.h"
#include "../../../Util/DataStructure/Bindable.h"
#include "../../Sheetmusic/SheetmusicManager.h"


using namespace Base::Sheetmusics;
using namespace Framework::Scenes;
using namespace Util::DataStructure;


namespace Base {
namespace Scenes {
namespace Select {

	/// <summary>
	/// ��update�ˬd�q�����S���Q����A�p�G�Q������ܡA�N�I�scarouselSelectionChanged�A
	/// ��ܧ�����A�|�Nplayer loader Ū���i�h�A�åBpush player
	/// </summary>
	class SongSelect : public Scene {


	public:

		SongSelect();

		/// <summary>
		/// ��ܧ����A�i�J�U�ӳ���
		/// </summary>
		virtual int TriggerOnSelected();

	protected:

		Bindable<WorkingSm*> workingSm;

		SmManager* smManager;

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
		//int load();

		/// <summary>
		/// ��ܺq������檺�{��
		/// </summary>
		int selectionChanged(SmInfo* sheetmusicInfo);
	};

}}}





#endif