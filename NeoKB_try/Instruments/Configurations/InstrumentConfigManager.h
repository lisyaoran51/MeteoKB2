#ifndef INSTRUMENT_CONFIG_MANAGER_H
#define INSTRUMENT_CONFIG_MANAGER_H


#include "../../Framework/Configurations/ConfigManager.h"

using namespace Util;
using namespace std;
using namespace Framework::Configurations;



namespace Instruments {
namespace Configurations {

	enum class InstrumentSetting;

	class InstrumentConfigManager: public TConfigManager<InstrumentSetting> {

	public:

		InstrumentConfigManager();

		virtual ~InstrumentConfigManager() = default;

	protected:

		virtual int initializeDefault();

		virtual int setNamesOfConfig();


	};


	enum class InstrumentSetting {

		/// <summary>
		/// 開機時的沿音模式，
		/// 0:None
		/// 1:SustainPedal
		/// 2:AutoSustain
		/// 3:GameControllingSustain
		///	</summary>
		InitialSustainType,

		/// <summary>
		/// 開機時的音色
		///	</summary>
		InitialSoundBankName,

		/// <summary>
		/// 將琴鍵音量開幾個根號，預設是二個根號，開三根就會更大聲
		///	</summary>
		SensitiveLevel,



	};

}}






#endif