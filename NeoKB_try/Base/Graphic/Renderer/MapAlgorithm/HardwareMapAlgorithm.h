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
		/// 在建立物鍵的時候，他會自動設好自己可以相容的硬體版本
		/// TODO: 應該要從文件檔去讀取每個硬體相容哪些mapping方法
		/// </summary>
		virtual int setCompatibleHwVersion();

	public:

		HardwareMapAlgo();

		int RegisterMap(Map* m);

		/// <summary>
		/// 註冊輸出的對象
		/// TODO: 還不確定輸出的對象是什麼形態，應該式陣列
		/// </summary>
		//int RegisterOutput();

		bool CheckHardwareVersion(int hwVersion);

	protected:

		Map* lightMap;

	};




}}}}




#endif