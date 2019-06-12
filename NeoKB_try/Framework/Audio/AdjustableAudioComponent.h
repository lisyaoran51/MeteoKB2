#ifndef ADJUSTABLE_AUDIO_COMPONENT_H
#define ADJUSTABLE_AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 裡面可以加入資源T
	/// TODO: 記成adjust aduio component，必須要是updatable才能update
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