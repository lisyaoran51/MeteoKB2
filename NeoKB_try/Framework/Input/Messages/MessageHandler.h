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
		/// 讓input manager決定要不要丟message給這個物件
		/// </summary>
		bool GetIsMessageActive() {
			return isMessageActive;
		}

	protected:

		/// <summary>
		/// 這個是讓繼承的人決定要不要收到message
		/// </summary>
		bool isMessageActive = true;

	};


}}}




#endif