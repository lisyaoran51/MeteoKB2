#ifndef INDICATOR_LIGHT_PANEL_MESSAGE_H
#define INDICATOR_LIGHT_PANEL_MESSAGE_H


#include "../../../Framework/Output/Panels/PanelMessage.h"
#include <vector>


using namespace Framework::Output::Panels;
using namespace std;



namespace Games {
namespace Output{
namespace Panels{

	enum class IndicatorLight {
		Power,
		Sensitivity,
		Sustain,
		LowerOctave,
		RaiseOctave
	};

	class IndicatorLightPanelMessage : public PanelMessage {

	public:

		/// <summary>
		/// 
		/// Light number:
		/// -1: turn off all lights
		/// 0: power
		/// 1: sentivity
		/// 2: sustain
		/// 3: raise octave
		///    *13
		/// *8    *12
		/// *9    *11
		///    *10
		/// 14: lower octave
		/// 
		///		
		/// 		*2 *1 *0
		/// *14		      *3
		///		
		/// </summary>
		IndicatorLightPanelMessage(int p, bool l);

		IndicatorLightPanelMessage(IndicatorLight iLight, bool l);

		virtual string ToString();

	protected:

		int position = -1;
		bool isLightUp = false;


	};

}}}









#endif