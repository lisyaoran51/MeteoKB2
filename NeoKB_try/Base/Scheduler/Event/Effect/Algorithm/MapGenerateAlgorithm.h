#ifndef MAP_GENERATE_ALGORITHM_H
#define MAP_GENERATE_ALGORITHM_H

#include"../../../../Graphic/Map/Map.h"
#include "../../Event.h"
#include "../../EventProcessor.h"

using namespace Base::Graphic::Maps;


namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {

	template<typename T>
	class EffectMapper;

}}}}

using namespace Base::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events;

namespace Base {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class MapGenerateAlgorithmInterface {

	public:

		// EventProcessor<Event> = EffectMapper<T>
		// 因為template不能做多型，所以多切一層interface出來
		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int Generate(Map* m, EventProcessor<Event>* em) = 0;
		
	};


	/// <summary>
	/// 畫出特效
	///	</summary>
	template<typename T>
	class MapGenerateAlgorithm: public MapGenerateAlgorithmInterface
	{
		
	public:
		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int Generate(Map* m, EventProcessor<Event>* em) {
			if (em->CanCast<EffectMapper<T>>()) {
				return ImplementGenerate(m, em->Cast<EffectMapper<T>>());
			}
			// TODO: throw error or not?
			return -1;
		}

	protected:
		/// <summary>
		/// 把evnet的狀態轉成圖案
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<T>* em) = 0;


	};
	

}}}}}








#endif