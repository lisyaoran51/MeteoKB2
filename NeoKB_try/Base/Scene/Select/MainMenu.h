#ifndef MAIN_MENU_H
#define MAIN_MENU_H


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
	class MainMenu : public Scene {

	public:



	protected:


	private:

		/// <summary>
		/// Ū��BeatmapManager beatmaps, AudioManager audio, DialogOverlay dialog, OsuGame osu, OsuColour colours
		/// </summary>
		int load();


		/// <summary>
		/// Ū��
		/// </summary>
		//int load();

	};

}}}





#endif