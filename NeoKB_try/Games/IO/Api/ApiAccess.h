#ifndef API_ACCESS_H
#define API_ACCESS_H

#include <functional>
#include "../../../Util/DataStructure/ActionList.h"
#include "../../../Framework/Host/GameHost.h"
#include "../../Users/User.h"


using namespace std;
using namespace Util::DataStructure;
using namespace Framework::Host;
using namespace Games::Users;


namespace Games {
namespace IO{
namespace Api{

	class ApiAccess : public MtoObject {

	public:

		ApiAccess(GameHost* gHost);

		/// <summary>
		/// 寫入sm檔時的callback
		/// </summary>
		int AddOnStartWritingSmFile(MtoObject* callableObject, function<int(string)> callback, string name = "HandleReceivingNewSmFile");
		int AddOnWriteSmFileSuccess(MtoObject* callableObject, function<int(string)> callback, string name = "HandleNewSmFileReceived");
		// on sm file success, import


		/// <summary>
		/// 寫入新音色時的callback
		/// </summary>
		int AddOnStartWritingSoundFilePackage(MtoObject* callableObject, function<int(string)> callback, string name = "HandleReceivingNewSoundFilePackage");
		int AddOnWriteSoundFilePackageSuccess(MtoObject* callableObject, function<int(string)> callback, string name = "HandleNewSoundFilePackageReceived");
		// on sound file success, add to new sound binding
		

		/// <summary>
		/// 寫入新主程式時的callback
		/// </summary>
		int AddOnStartWritingProgram(MtoObject* callableObject, function<int(string)> callback, string name = "HandleReceivingNewProgram");
		int AddOnWriteProgramSuccess(MtoObject* callableObject, function<int(string)> callback, string name = "HandleNewProgramReceived");

	protected:

		GameHost* gameHost = nullptr;

		User* user = nullptr;

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