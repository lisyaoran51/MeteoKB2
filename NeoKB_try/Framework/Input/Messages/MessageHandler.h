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

		int OnMessage(T* command) {
			if (isMessageActive)
				onMessage(command);
		}

		/// <summary>
		/// 讓input manager決定要不要丟message給這個物件，目前好像沒有參考到?
		/// </summary>
		bool GetIsMessageActive() {
			return isMessageActive;
		}

	protected:

		/// <summary>
		/// 這個是讓繼承的人決定要不要收到message
		/// </summary>
		bool isMessageActive = true;

		virtual int onMessage(T* command) = 0;


	};


}}}




#endif