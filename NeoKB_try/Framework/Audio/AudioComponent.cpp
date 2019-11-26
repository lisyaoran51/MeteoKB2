#include "AudioComponent.h"

using namespace Framework::Audio;


bool AudioComponent::GetIsCompleted()
{
	return isCompleted;
}

int AudioComponent::Update()
{
	pendingActions.TriggerThenClear();
	return 0;
}

int AudioComponent::_DebugPrintComponents(string spaces)
{
	LOG(LogLevel::Debug) << spaces << "[" << this << "]";
	return 0;
}

int AudioComponent::Dispose()
{
	return 0;
}
