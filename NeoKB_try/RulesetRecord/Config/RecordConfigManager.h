#ifndef RECORD_CONFIG_MANAGER_H
#define RECORD_CONFIG_MANAGER_H

#include "../../Framework/Configurations/FrameworkConfigManager.h"

using namespace Util;
using namespace std;
using namespace Framework::Configurations;


namespace Record {
namespace Config{

	enum class RecordSetting;

	class RecordConfigManager : public TConfigManager<RecordSetting> {

	public:

		RecordConfigManager();

		virtual ~RecordConfigManager() = default;

	protected:

		virtual int initializeDefault();

		virtual int setNamesOfConfig();

	};


	enum class RecordSetting {

		PatternGenerator,
	};

}}



#endif