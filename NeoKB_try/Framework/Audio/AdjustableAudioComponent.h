#ifndef ADJUSTABLE_AUDIO_COMPONENT_H
#define ADJUSTABLE_AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>
#include "AudioComponent.h"
#include "../../Util/DataStructure/Bindable.h"
#include <set>


using namespace std;
using namespace Util::DataStructure;


namespace Framework {
namespace Audio {

	enum class AdjustableProperty;

	/// <summary>
	/// �̭��i�H�[�J�귽T
	/// TODO: �O��adjust aduio component�A�����n�Oupdatable�~��update
	/// </summary>
	class AdjustableAudioComponent : public AudioComponent {

	public:

		AdjustableAudioComponent();

		int AddAdjustmentDependency(AdjustableAudioComponent* adjustableAudioComponent);

		int AddAdjustment(AdjustableProperty type, Bindable<double>* adjustBindable);

		int RemoveAdjustment(AdjustableProperty type, Bindable<double>* adjustBindable);

		int InvalidateState();

		virtual int OnStateChange();

		Bindable<double>* GetVolume();

		Bindable<double>* GetVolumeCalculated();
		Bindable<double>* GetBalanceCalculated();
		Bindable<double>* GetFrequencyCalculated();

	protected:

		Bindable<double>* volume = new Bindable<double>(1.0);

		Bindable<double>* volumeCalculated = new Bindable<double>(1.0);
		Bindable<double>* balanceCalculated = new Bindable<double>(1.0);
		Bindable<double>* frequencyCalculated = new Bindable<double>(1.0);


		set<Bindable<double>*> volumeAdjustments;
		set<Bindable<double>*> balanceAdjustments;
		set<Bindable<double>*> frequencyAdjustments;

	private:

	};

	enum class AdjustableProperty
	{
		Volume,
		Balance,
		Frequency
	};

}}


#endif