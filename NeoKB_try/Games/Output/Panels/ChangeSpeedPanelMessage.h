#ifndef CHANGE_SPEED_PANEL_MESSAGE_H
#define CHANGE_SPEED_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"


using namespace Framework::Output::Panels;


namespace Games {
namespace Output{
namespace Panels{

	class ChangeSpeedPanelMessage : public PanelMessage {

		string roundAndToString(float number, int digit);

	public:

		/// <summary>
		/// 目前速度
		/// </summary>
		ChangeSpeedPanelMessage(float s);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:

		float speed = 0;


	};

}}}









#endif