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
		/// �}���ɪ��u���Ҧ��A
		/// 0:None
		/// 1:SustainPedal
		/// 2:AutoSustain
		/// 3:GameControllingSustain
		///	</summary>
		InitialSustainType,

		/// <summary>
		/// �}���ɪ�����
		///	</summary>
		InitialSoundBankName,

		/// <summary>
		/// �N�^�䭵�q�}�X�Ӯڸ��A�w�]�O�G�Ӯڸ��A�}�T�ڴN�|��j�n
		///	</summary>
		SensitiveLevel,



	};

}}






#endif