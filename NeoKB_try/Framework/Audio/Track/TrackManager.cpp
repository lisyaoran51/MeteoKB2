#include "TrackManager.h"

#include "RateSettableBassTrack.h"

using namespace Framework::Audio::Tracks;


TrackManager::TrackManager(CompositeResourceStore<char*>* rStore)
{
	resourceStore = rStore;

}

Track * TrackManager::GetTrack(string name)
{
	//Track* track = new BassTrack((char*)name.c_str());
	Track* track = new RateSettableBassTrack((char*)name.c_str());
	AddItem(track);
	return track;
}
