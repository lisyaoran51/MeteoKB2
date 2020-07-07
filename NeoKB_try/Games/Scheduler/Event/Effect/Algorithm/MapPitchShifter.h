#ifndef MAP_PITCH_SHIFTER_H
#define MAP_PITCH_SHIFTER_H



#include "../../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../../Instruments/Pitch.h"
#include "MapAlgorithm.h"



using namespace Framework::Allocation::Hierachal;
using namespace Instruments;


namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class MapPitchShifter : public Container {

		int load();

		int load(FrameworkConfigManager* f);

	public:

		MapPitchShifter();

		virtual int LazyConstruct(map<string, MapAlgorithmInterface*>* mapAlgorithms);

		virtual bool GetIsConstructed();

		virtual int JumpToPitch(Pitch p) = 0;

		virtual int SetSeekPitch(Pitch p) = 0;

		virtual int ResetSeekSpeed() = 0;

		virtual int SetSeekSpeed(double sSpeed) = 0;

		/// <summary>
		/// �]���p�G��frame time�^�������t�ת��ܡA�����ɭԭ�n����frame�S�O�֩κC�A���X�Ӫ��t�״N�|�X��
		/// �ҥH�令�������Frame���F�h���A�H�ɭ��s�p��t�סA�~�|�����
		/// </summary>
		virtual int SetFloatSeekSpeed(double sDistanceLeft, double sTimeLeft) = 0;

		virtual bool GetIsShifting() = 0;

		virtual Pitch GetTempPitch() = 0;

	protected:

		bool isConstructed = false;

		map<string, MapAlgorithmInterface*>* mapAlgos;

		Pitch defaultStartPitch;

		Pitch upperBound;

		Pitch lowerBound;

		int width;

		int height;

		bool pitchJumped = false;

		virtual int update();

		/// <summary>
		/// �����A�^�ǭȬO0�N��o�������ʡA�^�ǭȤ��O0�N�O�S������
		/// </summary>
		virtual int shift() = 0;

	};



}}}}}




#endif