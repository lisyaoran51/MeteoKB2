#ifndef SPEED_RING_PANEL_MESSAGE_H
#define SPEED_RING_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"
#include <vector>


using namespace Framework::Output::Panels;
using namespace std;



namespace Games {
namespace Output{
namespace Panels{

	class SpeedRingPanelMessage : public PanelMessage {

	public:

		/// <summary>
		///		*0
		/// *5		*1
		/// *4		*2
		///		*3
		/// </summary>
		SpeedRingPanelMessage(vector<bool> l);


		/// <summary>
		/// positive speed
		///    *0
		/// *     *1
		/// *4    *2
		///    *3
		/// negative speed
		///    *0
		/// *-1   *
		/// *-2   *-4
		///    *-3
		/// 
		/// 5: reset
		/// 
		/// </summary>
		SpeedRingPanelMessage(int p);

		virtual string ToString();

		virtual OutputMessage* Clone();

	protected:

		vector<bool> lights;

		int position = -5;

	};

}}}









#endif