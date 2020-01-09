#include "AdjustableAudioComponent.h"

using namespace Framework::Audio;


AdjustableAudioComponent::AdjustableAudioComponent()
{
	volume->AddOnValueChanged(this, bind(&AdjustableAudioComponent::OnStateChange, this), "AdjustableAudioComponent::OnStateChange");
}

int AdjustableAudioComponent::AddAdjustmentDependency(AdjustableAudioComponent * adjustableAudioComponent)
{
	AddAdjustment(AdjustableProperty::Volume, adjustableAudioComponent->GetVolumeCalculated());

	return 0;
}

int AdjustableAudioComponent::AddAdjustment(AdjustableProperty type, Bindable<double>* adjustBindable)
{
	switch (type) {
	case AdjustableProperty::Volume:
		volumeAdjustments.insert(adjustBindable);
	}

	InvalidateState();

	return 0;
}

int AdjustableAudioComponent::RemoveAdjustment(AdjustableProperty type, Bindable<double>* adjustBindable)
{
	// TODO: 有需要再寫
	return 0;
}

int AdjustableAudioComponent::InvalidateState()
{
	pendingActions.Add(this, bind(&AdjustableAudioComponent::OnStateChange, this), "AdjustableAudioComponent::OnStateChange");
	return 0;
}

int AdjustableAudioComponent::OnStateChange()
{
	/* volume */
	double aggregateVolume = volume->GetValue();
	set<Bindable<double>*>::iterator iter;
	for (iter = volumeAdjustments.begin(); iter != volumeAdjustments.end(); ++iter)
	{
		aggregateVolume *= (*iter)->GetValue();
	}

	volumeCalculated->SetValue(aggregateVolume);

	return 0;
}

Bindable<double>* AdjustableAudioComponent::GetVolume()
{
	return volume;
}

Bindable<double>* AdjustableAudioComponent::GetVolumeCalculated()
{
	return volumeCalculated;
}

Bindable<double>* AdjustableAudioComponent::GetBalanceCalculated()
{
	return balanceCalculated;
}

Bindable<double>* AdjustableAudioComponent::GetFrequencyCalculated()
{
	return frequencyCalculated;
}
