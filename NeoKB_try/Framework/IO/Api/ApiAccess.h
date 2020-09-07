#ifndef API_ACCESS_H
#define API_ACCESS_H

#include <functional>
#include "../../../Util/DataStructure/ActionList.h"
#include "../../Host/GameHost.h"


using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Host;


namespace Framework {
namespace IO{
namespace Api{

	class ApiAccess : public MtoObject {

	public:

		ApiAccess(GameHost* gHost);

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSmFile(_Type* callableObject, function<int(string)> callback, string name = "HandleReceivingNewSmFile") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSmFile(callableObject, callback, name);
		}

		template<class _Type>
		int AddOnWriteSmFileSuccess(_Type* callableObject, function<int(string)> callback, string name = "HandleNewSmFileReceived") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSmFileSuccess(callableObject, callback, name);
		}
		// on sm file success, import


		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingSoundFilePackage(_Type* callableObject, function<int(string)> callback, string name = "HandleReceivingNewSoundFilePackage") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
		}

		template<class _Type>
		int AddOnWriteSoundFilePackageSuccess(_Type* callableObject, function<int(string)> callback, string name = "HandleNewSoundFilePackageReceived") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
		}
		// on sound file success, add to new sound binding
		

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		template<class _Type>
		int AddOnStartWritingProgram(_Type* callableObject, function<int(string)> callback, string name = "HandleReceivingNewProgram") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingProgram(callableObject, callback, name);
		}

		template<class _Type>
		int AddOnWriteProgramSuccess(_Type* callableObject, function<int(string)> callback, string name = "HandleNewProgramReceived") {
			return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteProgramSuccess(callableObject, callback, name);
		}

	protected:

		GameHost* gameHost = nullptr;

		ActionList<int, string> onStartWritingSmFile;
		ActionList<int, string> onWriteSmFileSuccess;

		ActionList<int, string> onStartWritingSoundFilePackage;
		ActionList<int, string> onWriteSoundFilePackageSuccess;

		ActionList<int, string> onStartWritingProgram;
		ActionList<int, string> onWriteProgramSuccess;

	};

}
}
}




#endif