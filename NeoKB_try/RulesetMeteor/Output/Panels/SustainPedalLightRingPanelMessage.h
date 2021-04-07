#ifndef SUSTAIN_PEDAL_LIGHT_RING_PANEL_MESSAGE_H
#define SUSTAIN_PEDAL_LIGHT_RING_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"


using namespace Framework::Output::Panels;


namespace Meteor {
namespace Output{
namespace Panels{

	class SustainPedalLightRingPanelMessage : public PanelMessage {

		string roundAndToString(float number, int digit);

	public:

		/// <summary>
		/// targetLifeTime是目標點會亮著多久，目標點亮完以後就開始畫光圈，也就是踏踏板的時機，光圈總共要轉ringLifeTime那麼九
		/// 在nextTargetStartTime則是跟轉光圈同時開始跑，有時候ringLifeTime結束了，nextTargetStartTime還沒到，光圈就會結束，這時光圈就只會亮到ringLifeTime/nextTargetStartTime圈
		/// 時間：targetLifeTime -> ringLifeTime(可能提早結束)
		///						 -> nextTargetStartTime
		/// </summary>
		SustainPedalLightRingPanelMessage(float tLifeTime, float pDownLifeTime, float rLifeTime);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:


		float targetLifeTime = 0;

		float pedalDownLifeTime = 0;

		float ringLifeTime = 0;


	};

}}}









#endif