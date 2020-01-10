#ifndef SAMPLE_CHANNEL_H
#define SAMPLE_CHANNEL_H

#include "../AudioCollectionManager.h"
#include "Sample.h"


namespace Framework {
namespace Audio {
namespace Samples {

	class SampleChannel : public AdjustableAudioComponent {


	public:

		//SampleChannel();

		SampleChannel* GetSampleChannel(string name);

		virtual int Play();

		virtual int Play(double v);

		virtual int Stop();

		virtual int FadeOut() = 0;

		virtual int StopFadeOut() = 0;

		/// <summary>
		/// fadeout多久，單位是秒
		/// </summary>
		int SetFadeOutTime(float fOutTime);

	protected:

		double initialVolume = 0;

		/// <summary>
		/// fadeout多久，單位是秒
		/// </summary>
		float fadeOutTime = 0.2f;

		SampleChannel(Sample* s);

		Sample* sample = nullptr;

		bool isPlaying = false;

		bool isStarted = false;

		bool isCompleted = false;

	};


}}}





#endif