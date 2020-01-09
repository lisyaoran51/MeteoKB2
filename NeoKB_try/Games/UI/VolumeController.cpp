#include "VolumeController.h"


using namespace Games::UI;



int VolumeController::load()
{
	AudioManager * a = GetCache<AudioManager>("AudioManager");
	if (!a)
		throw runtime_error("int VolumeController::load() : AudioManager not found in cache.");

	return load(a);
}

int VolumeController::load(AudioManager * aManager)
{
	masterVolumeMeter->BindTo(aManager->GetVolume());
	trackVolumeMeter->BindTo(aManager->GetTrackVolume());
	sampleVolumeMeter->BindTo(aManager->GetSampleVolume());

	return 0;
}

VolumeController::VolumeController(): RegisterType("VolumeController")
{
	registerLoad(bind(static_cast<int(VolumeController::*)(void)>(&VolumeController::load), this));
}

int VolumeController::onSlide(InputState * inputState, InputKey slider)
{
	
	if (slider == InputKey::MusicVolumeSlider) {
		int value = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetSliders()->size(); i++) {
			if (inputState->GetPanelState()->GetSliders()->at(i).first == slider) {
				value = inputState->GetPanelState()->GetSliders()->at(i).second;
			}
		}

		trackVolumeMeter->SetValue(value);
	}

	if (slider == InputKey::PianoVolumeSlider) {
		int value = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetSliders()->size(); i++) {
			if (inputState->GetPanelState()->GetSliders()->at(i).first == slider) {
				value = inputState->GetPanelState()->GetSliders()->at(i).second;
			}
		}

		sampleVolumeMeter->SetValue(value);
	}

	return 0;
}
