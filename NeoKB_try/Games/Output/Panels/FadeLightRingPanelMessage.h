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
		/// �o�ӬO���G���t������Alife time�N�O�q�G��t��h�[�ɶ�
		/// </summary>
		FadeLightRingPanelMessage(float lTime);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:

		float lifeTime = 0;


	};

}}}









#endif