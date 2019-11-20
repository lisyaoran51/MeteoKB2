#ifndef ADJUSTABLE_AUDIO_COMPONENT_H
#define ADJUSTABLE_AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>
#include "AudioComponent.h"
#include "Scene/Loader.h"
#include "../Util/DataStructure/Bindable.h"


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

		int AddAdjustment(AdjustableProperty type, Bindable<double> adjustBindable);

		int RemoveAdjustment(AdjustableProperty type, Bindable<double> adjustBindable);


	protected:


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