#ifndef METEO_CONFIG_MANAGER_H
#define METEO_CONFIG_MANAGER_H


#include "../../Framework/Configurations/ConfigManager.h"




using namespace Framework::Configurations;


namespace Games {
namespace Configurations{

	enum class MeteoSetting;

	class MeteoConfigManager : public TConfigManager<MeteoSetting> {

	public:

		MeteoConfigManager();

	protected:

		virtual int InitializeDefault();



	};

	enum class MeteoSetting {

		Ruleset,
		Username,
		AudioOffset,
		DefaultPianoVolume

	};







}}


#endif