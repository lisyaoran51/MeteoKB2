#ifndef GAME_H
#define GAME_H

namespace Framework {

	
	class Game : public Triggerable {

		AudioManager* audio;

		ResourceStore<char*>* resources;

		int load();

	public:

		virtual int SetHost(GameHost* host);

	};

}







#endif