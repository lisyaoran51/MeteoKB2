#ifndef ADJUSTABLE_AUDIO_COMPONENT_H
#define ADJUSTABLE_AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// �̭��i�H�[�J�귽T
	/// TODO: �O��adjust aduio component�A�����n�Oupdatable�~��update
	/// </summary>
	template<T>
	class AdjustableAudioComponent : AudioCompoennt {

	public:

		AdjustableAudioComponent();

		int AddAdjustment(AdjustableProperty type, BindableDouble adjustBindable);

		int RemoveAdjustment(AdjustableProperty type, BindableDouble adjustBindable);


	protected:


	private:

	};

	enum AdjustableProperty
	{
		Volume,
		Balance,
		Frequency
	};

}}


#endif