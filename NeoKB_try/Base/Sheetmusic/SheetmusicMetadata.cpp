#include "SheetmusicMetadata.h"

using namespace Base::Sheetmusics;

SmMetadata::SmMetadata()
{
}

SmMetadata::SmMetadata(SmMetadata * sm)
{
	Artist = sm->Artist;
	ArtistUnicode = sm->ArtistUnicode;
	AudioFile = sm->AudioFile;
	Author = sm->Author;

	OnlineSheetmusic = sm->OnlineSheetmusic;
	PreviewTime = sm->PreviewTime;

	Source = sm->Source;
	Tags = sm->Tags;
	Title = sm->Title;
	TitleUnicode = sm->TitleUnicode;

}
