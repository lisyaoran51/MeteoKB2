#ifndef LIGHT_RING_PANEL_MESSAGE_H
#define LIGHT_RING_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"


using namespace Framework::Output::Panels;


namespace Games {
namespace Output{
namespace Panels{

	class LightRingPanelMessage : public PanelMessage {

		string roundAndToString(float number, int digit);

	public:

		/// <summary>
		/// targetLifeTime�O�ؼ��I�|�G�ۦh�[�A�ؼ��I�G���H��N�}�l�e����A�]�N�O���O���ɾ��A�����`�@�n��ringLifeTime����E
		/// �bnextTargetStartTime�h�O�������P�ɶ}�l�]�A���ɭ�ringLifeTime�����F�AnextTargetStartTime�٨S��A����N�|�����A�o�ɥ���N�u�|�G��ringLifeTime/nextTargetStartTime��
		/// �ɶ��GtargetLifeTime -> ringLifeTime(�i�ണ������)
		///						 -> nextTargetStartTime
		/// </summary>
		LightRingPanelMessage(float lTime);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:

		float lifeTime = 0;

		float targetLifeTime = 0;

		float pedalDownLifeTime = 0;

		float ringLifeTime = 0;


	};

}}}









#endif