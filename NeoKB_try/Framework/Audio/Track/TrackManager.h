#ifndef TRACK_MANAGER_H
#define TRACK_MANAGER_H


#include "../AudioCollectionManager.h"
#include "Track.h"
#include "../../IO/Stores/CompositeResourceStore.h"


using namespace Framework::IO::Stores;

namespace Framework {
namespace Audio {
namespace Tracks {

	class TrackManager : public AudioCollectionManager<Track> {

	public:

		TrackManager(CompositeResourceStore<char*>* rStore);

		Track* GetTrack(string name);

	private:

		CompositeResourceStore<char*>* resourceStore;

	};


}}}



#endif