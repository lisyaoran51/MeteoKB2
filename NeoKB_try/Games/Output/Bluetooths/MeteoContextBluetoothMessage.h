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
		/// �Ȯɤ����readwrite�A���˷|���I�����A�����n��
		/// </summary>
		virtual int SetAccessType(MeteoBluetoothMessageAccessType aType);

		/// <summary>
		/// �u��set�̦��A�p�G�ĤG��set�N�|�⤧�eset�����e���R��
		/// </summary>
		virtual int SetContext(char* c, int cSize);

		/// <summary>
		/// �u��set�̦��A�p�G�ĤG��set�N�|�⤧�eset�����e���R��
		/// </summary>
		int SetContextInJson(json cInJson);

		string GetContext();

		/// <summary>
		/// �o��|��context in json�ƻs�@���A���X�h�A�ҥH�L�k�ק���l��context in json
		/// </summary>
		json GetContextInJson();

	protected:

		json contextInJson;

		char* context = nullptr;

		int contextSize = 0;

	};

}}}





#endif