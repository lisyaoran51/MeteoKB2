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
	/// ��update�ˬd�q�����S���Q����A�p�G�Q������ܡA�N�I�scarouselSelectionChanged�A
	/// ��ܧ�����A�|�Nplayer loader Ū���i�h�A�åBpush player
	/// </summary>
	class SongSelect : public Scene {

	public:



	protected:

		/// <summary>
		/// �b��ܺq��carouselSelectionChanged�ɰ���A�|��C���Ҧ��M�иj�b�@�_
		/// </summary>
		int updateSheetmusic(WorkingSm* workingSm);

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
		int carouselSelectionChanged(SmInfo* sheetmusicInfo);
	};

}}}





#endif