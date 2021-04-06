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
	mirrorSampleVolumeMeter->BindTo(aManager->GetMirrorSampleVolume());

	isPresent = true;

	return 0;
}

VolumeController::VolumeController(): RegisterType("VolumeController")
{
	registerLoad(bind(static_cast<int(VolumeController::*)(void)>(&VolumeController::load), this));

	isInputable = true;

}

int VolumeController::onSlide(InputState * inputState, InputKey slider)
{
	LOG(LogLevel::Debug) << "VolumeController::onSlide() : start sliding.";
	if (slider == InputKey::MusicVolumeSlider) {
		int value = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetSliders()->size(); i++) {
			if (inputState->GetPanelState()->GetSliders()->at(i).first == slider) {
				value = inputState->GetPanelState()->GetSliders()->at(i).second;
			}
		}

		LOG(LogLevel::Debug) << "VolumeController::onSlide() : music volume slide to [" << (float)value / 100.f << "].";
		mirrorSampleVolumeMeter->SetValue((float)value / 100.f);
	}

	if (slider == InputKey::PianoVolumeSlider) {
		int value = 0;
		for (int i = 0; i < inputState->GetPanelState()->GetSliders()->size(); i++) {
			if (inputState->GetPanelState()->GetSliders()->at(i).first == slider) {
				value = inputState->GetPanelState()->GetSliders()->at(i).second;
			}
		}

		LOG(LogLevel::Debug) << "VolumeController::onSlide() : piano volume slide to [" << (float)value / 100.f << "].";
		sampleVolumeMeter->SetValue((float)value / 100.f);
	}

	return 0;
}
