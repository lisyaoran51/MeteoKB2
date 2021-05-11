#ifndef GAME_HOST_H
#define GAME_HOST_H

#include "../../Framework/Hosts/Host.h"

using namespace Framework::Hosts;

namespace Games {
namespace Hosts {

	class GameHost : public Host {

	public:

		GameHost();

		virtual int SetupOutputManager(OutputManager* oManager);

	protected:

		virtual vector<InputHandler*>* createAvailableInputHandlers();

		virtual int setupConfig();

	};


}}



#endif