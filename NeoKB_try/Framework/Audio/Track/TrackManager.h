#ifndef TRACK_MANAGER_H
#define TRACK_MANAGER_H


#include "../AudioCollectionManager.h"
#include "Track.h"
#include "../../IO/Stores/ResourceStore.h"


using namespace Framework::IO::Stores;

namespace Framework {
namespace Audio {
namespace Track {

	class TrackManager : public AudioCollectionManager<Track> {

	public:

		TrackManager(ResourceStore<char*>* rStore);

		Track* GetTrack(string name);

	private:

		ResourceStore<char*>* resourceStore;

	};


}}}



#endif