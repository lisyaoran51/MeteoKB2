#ifndef LINEAR_MAP_PITCH_SHIFTER_H
#define LINEAR_MAP_PITCH_SHIFTER_H

#include "MapPitchShifter.h"
#include "../../../../../RulesetMeteor/Config/MeteorConfigManager.h"


using namespace Meteor::Config;


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

		int load(MeteorConfigManager* m);

	public:

		LinearMapPitchShifter();

		virtual int JumpToPitch(Pitch p);

		virtual int SetSeekPitch(Pitch p);

		virtual bool GetIsShifting();

		virtual int ResetSeekSpeed();

		virtual int SetSeekSpeed(double sSpeed);

		/// <summary>
		/// �]���p�G��frame time�^�������t�ת��ܡA�����ɭԭ�n����frame�S�O�֩κC�A���X�Ӫ��t�״N�|�X��
		/// �ҥH�令�������Frame���F�h���A�H�ɭ��s�p��t�סA�~�|�����
		/// </summary>
		virtual int SetFloatSeekSpeed(double sDistanceLeft, double sTimeLeft);

		/// <summary>
		/// �o�����ӭn�֥ΡAframe���t�פ��T�w
		/// </summary>
		int SetMovePerFrame(double mPerFrame);

		virtual Pitch GetTempPitch();

	protected:

		bool isShifting = false;

		Pitch tempPitch;

		Pitch pitchShiftingTo;

		float defaultMovePerFrame = 1.0;

		double movePerFrame = 1.0;

		double floatMovePerFrame = 1.0;

		double tempSeekDistanceLeft = 0;

		double tempSeekTimeLeft = 0;

		double tempPitchSpecificPosition;


		virtual int shift();

		double getFloatMovePerFrame();

		int resetFloatSeekSpeed();
	};


}}}}}



#endif
