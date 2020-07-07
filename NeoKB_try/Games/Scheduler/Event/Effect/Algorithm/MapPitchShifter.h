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
		/// 因為如果用frame time回推平移速度的話，有的時候剛好那個frame特別快或慢，推出來的速度就會出錯
		/// 所以改成紀錄秒個Frame走了多遠，隨時重新計算速度，才會比較準
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
		/// 平移，回傳值是0代表這次有移動，回傳值不是0就是沒有移動
		/// </summary>
		virtual int shift() = 0;

	};



}}}}}




#endif