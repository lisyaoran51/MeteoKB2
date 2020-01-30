#include "SongProgress.h"



using namespace Games::Scenes::Play::Hud;



SongProgress::SongProgress() : RegisterType("SongProgress"){

}

int SongProgress::SetTimeSource(AdjustableClock * tSource)
{
	timeSource = tSource;
	return 0;
}

AdjustableClock* SongProgress::GetTimeSource(){

	return timeSource;

}

int SongProgress::SetIsAllowSeek(bool iAllowSeek)
{
	return 0;
}
