#ifndef VOLUME_CONTROLLER_H
#define VOLUME_CONTROLLER_H


#include "../../Framework/Allocation/Hierachal/Container.h"
#include "../../Util/DataStructure/Bindable.h"
#include "../../Framework/Audio/AudioManager.h"


using namespace Framework::Allocation::Hierachal;
using namespace Util::DataStructure;
using namespace Framework::Audio;



namespace Games {
namespace UI {

	class VolumeController : public Container {

		int load();

		int load(AudioManager* aManager);


	public:

		VolumeController();

	protected:

		Bindable<double>* masterVolumeMeter = new Bindable<double>(1.0);
		Bindable<double>* sampleVolumeMeter = new Bindable<double>(1.0);
		Bindable<double>* trackVolumeMeter = new Bindable<double>(1.0);

		virtual int onSlide(InputState* inputState, InputKey slider);

	};

}}








#endif