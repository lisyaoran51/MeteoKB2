#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "BluetoothCommand.h"

using json = nlohmann::json;

namespace Framework {
namespace Input {
namespace Messages{

	template<typename T>
	class MessageHandler {

	public:

		virtual int OnMessage(T* command) = 0;

		/// <summary>
		/// ��input manager�M�w�n���n��message���o�Ӫ���
		/// </summary>
		bool GetIsMessageActive() {
			return isMessageActive;
		}

	protected:

		/// <summary>
		/// �o�ӬO���~�Ӫ��H�M�w�n���n����message
		/// </summary>
		bool isMessageActive = true;

	};


}}}




#endif