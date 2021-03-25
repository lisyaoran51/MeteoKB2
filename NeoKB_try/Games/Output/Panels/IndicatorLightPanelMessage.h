#ifndef INDICATOR_LIGHT_PANEL_MESSAGE_H
#define INDICATOR_LIGHT_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"
#include <vector>


using namespace Framework::Output::Panels;
using namespace std;



namespace Games {
namespace Output{
namespace Panels{

	class IndicatorLightPanelMessage : public PanelMessage {

	public:

		/// <summary>
		///		
		/// 		* * *
		/// *		    *
		///		
		/// </summary>
		IndicatorLightPanelMessage(int p, bool l);

		virtual string ToString();

	protected:

		int position = -1;
		bool isLightUp = false;


	};

}}}









#endif