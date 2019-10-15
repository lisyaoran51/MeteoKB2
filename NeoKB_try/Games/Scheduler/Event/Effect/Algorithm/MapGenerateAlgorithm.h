#ifndef MAP_GENERATE_ALGORITHM_H
#define MAP_GENERATE_ALGORITHM_H

#include "../../../../../Framework/Graphic/Map/Map.h"
#include "../../Event.h"
#include "../../EventProcessor.h"

using namespace Framework::Graphic::Maps;


namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {

	template<typename T>
	class EffectMapper;

}}}}

using namespace Games::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events;

namespace Games {
namespace Schedulers {
namespace Events {
namespace Effects {
namespace Algorithms{
	
	class MapGenerateAlgorithmInterface {

	public:

		// EventProcessor<Event> = EffectMapper<T>
		// �]��template���వ�h���A�ҥH�h���@�hinterface�X��
		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int Generate(Map* m, EventProcessor<Event>* em) = 0;
		
	};


	/// <summary>
	/// �e�X�S��
	///	</summary>
	template<typename T>
	class MapGenerateAlgorithm: public MapGenerateAlgorithmInterface
	{
		
	public:
		/// <summary>
		/// ��evnet�����A�ন�Ϯ�
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
		/// ��evnet�����A�ন�Ϯ�
		///	</summary>
		virtual int ImplementGenerate(Map* m, EffectMapper<T>* em) = 0;


	};
	

}}}}}








#endif