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

		virtual int SetSeekPitch(Pitch p) = 0;

		virtual bool GetIsShifting() = 0;

		virtual Pitch GetTempPitch() = 0;

	protected:

		bool isConstructed = false;

		map<string, MapAlgorithmInterface*>* mapAlgos;

		Pitch defaultStartPitch;

		int width;

		int height;

		virtual int update();

		/// <summary>
		/// 平移，回傳值是0代表這次有移動，回傳值不是0就是沒有移動
		/// </summary>
		virtual int shift() = 0;

	};



}}}}}




#endif