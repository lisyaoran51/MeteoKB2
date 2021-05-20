#ifndef FADE_LIGHT_RING_PANEL_MESSAGE_H
#define FADE_LIGHT_RING_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"


using namespace Framework::Output::Panels;


namespace Games {
namespace Output{
namespace Panels{

	class FadeLightRingPanelMessage : public PanelMessage {

		string roundAndToString(float number, int digit);

	public:

		/// <summary>
		/// 這個是漸亮漸暗的光圈，life time就是從亮到暗花多久時間
		/// </summary>
		FadeLightRingPanelMessage(float lTime);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:

		float lifeTime = 0;


	};

}}}









#endif