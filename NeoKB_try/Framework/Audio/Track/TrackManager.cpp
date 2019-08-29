#include "TrackManager.h"

#include "BassTrack.h"

using namespace Framework::Audio::Track;


TrackManager::TrackManager(ResourceStore<char*>* rStore)
{
	resourceStore = rStore;

}

Track * TrackManager::GetTrack(string name)
{
	Track* track = new BassTrack((char*)name.c_str());
	AddItem(track);
	return track;
}
