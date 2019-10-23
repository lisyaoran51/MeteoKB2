#ifndef LINEAR_MAP_PITCH_SHIFTER_H
#define LINEAR_MAP_PITCH_SHIFTER_H

#include "MapPitchShifter.h"




namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{

	/// <summary>
	/// �Ψӧ�map������Q�n��pitch���u��A���ʤ覡�O�u�ʲ���
	/// </summary>
	class LinearMapPitchShifter : public MapPitchShifter {

		int load();

	public:

		LinearMapPitchShifter();

		virtual int SetSeekPitch(Pitch p);

		virtual bool GetIsShifting();

		int SetMovePerFrame(double mPerFrame);

		virtual Pitch GetTempPitch();

	protected:

		bool isShifting = false;

		Pitch tempPitch;

		Pitch pitchShiftingTo;

		double movePerFrame = 1.0;

		double tempPitchSpecificPosition;


		virtual int shift();




	};


}}}}}



#endif
