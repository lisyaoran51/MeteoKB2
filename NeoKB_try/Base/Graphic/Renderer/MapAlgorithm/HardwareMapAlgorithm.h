#ifndef HARDWARE_MAP_ALGORITHM_H
#define HARDWARE_MAP_ALGORITHM_H

#include<vector> 
#include<map> 
#include "../../Map/Map.h"
#include "../../../../Util/MtoObject.h"


using namespace std;
using namespace Base::Graphic::Maps;
using namespace Util;



namespace Base {
namespace Graphic {
namespace Renderers {
namespace MapAlgorithms {

	class HardwareMapAlgo: public MtoObject {

		vector<int> compatibleHwVersion;

		/// <summary>
		/// �b�إߪ��䪺�ɭԡA�L�|�۰ʳ]�n�ۤv�i�H�ۮe���w�骩��
		/// TODO: ���ӭn�q����ɥhŪ���C�ӵw��ۮe����mapping��k
		/// </summary>
		virtual int setCompatibleHwVersion();

	public:

		HardwareMapAlgo();

		int RegisterMap(Map* m);

		/// <summary>
		/// ���U��X����H
		/// TODO: �٤��T�w��X����H�O����κA�A���Ӧ��}�C
		/// </summary>
		//int RegisterOutput();

		bool CheckHardwareVersion(int hwVersion);

	protected:

		Map* lightMap;

	};




}}}}




#endif