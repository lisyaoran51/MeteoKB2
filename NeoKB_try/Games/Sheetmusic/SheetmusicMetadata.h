#ifndef SHEETMUSIC_METADATA_H
#define SHEETMUSIC_METADATA_H

#include <string>
#include <fstream>
#include <vector>
#include "../../Util/TemplateConstraint.h"



using namespace std;
using namespace Util;

namespace Games {
namespace Sheetmusics {
	
	 
	struct SmMetadata
	{

		string Artist = "";
		string ArtistUnicode = "";
		bool hasAudioFile = false;
		string AudioFile = "";
		string Author = "";
		int OnlineSheetmusic;
		int PreviewTime;
		string Source = "";
		string Tags = "";
		string Title = "";
		string TitleUnicode = "";

		SmMetadata();
		SmMetadata(SmMetadata* sm);

	};


}}



#endif