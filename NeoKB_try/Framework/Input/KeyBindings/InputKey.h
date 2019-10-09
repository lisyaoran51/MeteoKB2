#ifndef INPUT_KEY_H
#define INPUT_KEY_H

namespace Framework {
namespace Input {
namespace KeyBindings {


	enum class InputKey {

		None = 0,

		/* Key */

		VK27_A1 = 100,
		VK27_bB1,
		VK27_B1,
		  
		VK27_C2,
		VK27_bD2,
		VK27_D2,
		VK27_bE2,
		VK27_E2,
		VK27_F2,
		VK27_bG2,
		VK27_G2,
		VK27_bA2,
		VK27_A2,
		VK27_bB2,
		VK27_B2,
		  
		VK27_C3,
		VK27_bD3,
		VK27_D3,
		VK27_bE3,
		VK27_E3,
		VK27_F3,
		VK27_bG3,
		VK27_G3,
		VK27_bA3,
		VK27_A3,
		VK27_bB3,
		VK27_B3,

		VK27_C4,
		VK27_bD4,
		VK27_D4,
		VK27_bE4,
		VK27_E4,
		VK27_F4,
		VK27_bG4,
		VK27_G4,
		VK27_bA4,
		VK27_A4,
		VK27_bB4,
		VK27_B4,

		VK24_L_C1,
		VK24_L_bD1,
		VK24_L_D1,
		VK24_L_bE1,
		VK24_L_E1,
		VK24_L_F1,
		VK24_L_bG1,
		VK24_L_G1,
		VK24_L_bA1,
		VK24_L_A1,
		VK24_L_bB1,
		VK24_L_B1,

		VK24_L_C2,
		VK24_L_bD2,
		VK24_L_D2,
		VK24_L_bE2,
		VK24_L_E2,
		VK24_L_F2,
		VK24_L_bG2,
		VK24_L_G2,
		VK24_L_bA2,
		VK24_L_A2,
		VK24_L_bB2,
		VK24_L_B2,

		VK24_R_C1,
		VK24_R_bD1,
		VK24_R_D1,
		VK24_R_bE1,
		VK24_R_E1,
		VK24_R_F1,
		VK24_R_bG1,
		VK24_R_G1,
		VK24_R_bA1,
		VK24_R_A1,
		VK24_R_bB1,
		VK24_R_B1,
			 
		VK24_R_C2,
		VK24_R_bD2,
		VK24_R_D2,
		VK24_R_bE2,
		VK24_R_E2,
		VK24_R_F2,
		VK24_R_bG2,
		VK24_R_G2,
		VK24_R_bA2,
		VK24_R_A2,
		VK24_R_bB2,
		VK24_R_B2,

		VK37_C1,
		VK37_bD1,
		VK37_D1,
		VK37_bE1,
		VK37_E1,
		VK37_F1,
		VK37_bG1,
		VK37_G1,
		VK37_bA1,
		VK37_A1,
		VK37_bB1,
		VK37_B1,
		  
		VK37_C2,
		VK37_bD2,
		VK37_D2,
		VK37_bE2,
		VK37_E2,
		VK37_F2,
		VK37_bG2,
		VK37_G2,
		VK37_bA2,
		VK37_A2,
		VK37_bB2,
		VK37_B2,

		VK37_C3,
		VK37_bD3,
		VK37_D3,
		VK37_bE3,
		VK37_E3,
		VK37_F3,
		VK37_bG3,
		VK37_G3,
		VK37_bA3,
		VK37_A3,
		VK37_bB3,
		VK37_B3,

		VK37_C4,

		/* Pedal */
		SustainPedal = 500,
		ExpressionPedal,


		/* Button */
		RaiseOctave = 1000,
		LowerOctave = 1001,

		Pause		= 1002,

		/* Knob */
		SectionKnob	= 1003,
		SpeedKnob	= 1004,

		/* Bluetooth */
		Bluetooth	= 1005,

		/* Slider */
		PianoVolume	= 1006,
		MusicVolume = 1007,

		/* General Button */
		Power		= 1008,
		Sustain		= 1009,
		Sensitivity	= 1010,

		/* Plugin */
		BluetoothPlugin = 1500,
		EarphonePlugin,
		SustainPedalPlugin,
		ExpressionPedalPlugin,

	};


}}}


#endif