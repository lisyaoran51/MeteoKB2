#ifndef REALTIME_REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H
#define REALTIME_REVERB_GRADIENT_TIMBRE_SIMPLE_SOUND_BINDING_H


#include "GradientTimbreSimpleSoundBinding.h"
#include <vector>


using namespace std;


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class RealtimeReverbGradientTimbreSimpleSoundBinding : public GradientTimbreSimpleSoundBinding<T> {

	public:

		RealtimeReverbGradientTimbreSimpleSoundBinding(string sBankName, T a): GradientTimbreSimpleSoundBinding<T>(sBankName, a){
			delayTimes.push_back(0);
			delayTimes.push_back(0.03);
			delayTimes.push_back(0.06);
			delayTimes.push_back(0.09);
			delayTimes.push_back(0.12);
			reverbVolumes.push_back(0.08);
			reverbVolumes.push_back(0.09);
			reverbVolumes.push_back(0.1);
			reverbVolumes.push_back(0.11);
			reverbVolumes.push_back(0.12);
		}

		int SetPredelay(float p) {
			if (p > 0.1)
				return 0;
			predelay = p;
		}

		int SetDelays(vector<float> dTimes) {
			if (dTimes.size() < 5)
				return 0;

			delayTimes.clear();
			delayTimes.assign(dTimes.begin(), dTimes.end());
		}

		int SetReverbVolumes(vector<float> rVolumes) {
			if (rVolumes.size() < 5)
				return 0;

			reverbVolumes.clear();
			reverbVolumes.assign(rVolumes.begin(), rVolumes.end());
		}


	protected:

		float predelay = 0.1f;

		vector<float> delayTimes;

		vector<float> reverbVolumes;

	};

}}



#endif