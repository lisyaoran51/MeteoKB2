#include "AudioComponent.h"

using namespace Framework::Audio;


bool AudioComponent::GetIsCompleted()
{
	return isCompleted;
}

int AudioComponent::update()
{
	pendingActions.TriggerThenClear();
	return 0;
}

int AudioComponent::Dispose()
{
	return 0;
}
