#include "TrackManager.h"

using namespace Framework::Audio::Track;


TrackManager::TrackManager(ResourceStore<char*>* rStore)
{
	resourceStore = rStore;

}

Track * TrackManager::GetTrack(string name)
{
	Track* track = new BassTrack(resourceStore->GetStream(name));
	AddItem(track);
	return track;
}
