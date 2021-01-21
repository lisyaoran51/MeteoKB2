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

		virtual BluetoothMessage* Clone();

		/// <summary>
		/// 暫時不能用readwrite，那樣會有點複雜，先不要用
		/// </summary>
		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		/// <summary>
		/// 只能set依次，如果第二次set就會把之前set的內容都刪掉
		/// </summary>
		virtual int SetContext(char* c, int cSize);

		/// <summary>
		/// 只能set依次，如果第二次set就會把之前set的內容都刪掉
		/// </summary>
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