#ifndef METEO_CONTEXT_BLUETOOTH_MESSAGE_H
#define METEO_CONTEXT_BLUETOOTH_MESSAGE_H


#include "MeteoBluetoothMessage.h"
#include "../../../ThirdParty/json/json.hpp"


using json = nlohmann::json;


namespace Games {
namespace Output{
namespace Bluetooths{

	class MeteoContextBluetoothMessage : public MeteoBluetoothMessage {


	public:

		MeteoContextBluetoothMessage(MeteoCommand mCommand);

		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		virtual int SetContext(char* c, int cSize);

		int SetContextInJson(json cInJson);

		string GetContext();

		/// <summary>
		/// 這邊會把context in json複製一份再拿出去，所以無法修改到原始的context in json
		/// </summary>
		json GetContextInJson();

	protected:

		json contextInJson;

		char* context = nullptr;

		int contextSize = 0;

	};

}}}





#endif