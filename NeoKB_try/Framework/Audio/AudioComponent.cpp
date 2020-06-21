#include "AudioComponent.h"

using namespace Framework::Audio;


bool AudioComponent::GetIsCompleted()
{
	return isCompleted;
}

int AudioComponent::Update()
{
	LOG(LogLevel::Depricated) << "AudioComponent::Update : this = [" << this << "] pending actions size = [" << pendingActions.GetSize() << "]";

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.TriggerThenClear();

	unique_lock<mutex> uLock(permanentActionMutex);
	permanentActions.Trigger();

	return 0;
}

int AudioComponent::SetClock(FrameBasedClock * c)
{
	clock = c;

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
