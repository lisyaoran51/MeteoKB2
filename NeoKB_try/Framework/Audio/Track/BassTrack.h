#ifndef BASS_TRACK_H
#define BASS_TRACK_H



#include <vector>
#include <thread>
#include <functional>
#include "Track.h"
#include "../../../ThirdParty/Bass/bass.h"


// https://stackoverflow.com/questions/7180920/bass-play-a-stream


using namespace std;
using namespace Framework::Timing;

namespace Framework {
namespace Audio {
namespace Tracks {

	/// <summary>
	/// 裡面有work queue，每次update就把工作做掉
	/// </summary>
	class BassTrack : public Track {

		HSTREAM stream;

	public:

		BassTrack(char* fileName);

		virtual int Start();

		virtual int Stop();

		virtual bool Seek(double position);

		virtual int Reset();

		virtual int Dispose();

		virtual int Update();

	protected:


	private:

		

	};

}}}


#endif