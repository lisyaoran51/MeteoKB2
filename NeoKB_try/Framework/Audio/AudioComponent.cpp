#include "AudioComponent.h"

using namespace Framework::Audio;


bool AudioComponent::GetIsCompleted()
{
	return isCompleted;
}

int AudioComponent::Update()
{
	LOG(LogLevel::Depricated) << "AudioComponent::Update : this = [" << this << "] pending actions size = [" << pendingActions.GetSize() << "]";

	if (pendingActions.GetSize() > 0) {
		unique_lock<mutex> uLock(pendingActionMutex);
		pendingActions.TriggerThenClear();
	}

	if (permanentActions.GetSize() > 0) {
		unique_lock<mutex> uLock2(permanentActionMutex);
		permanentActions.Trigger();
	}
	
	if (timedActions.size() > 0) {
		vector<pair<float, function<int()>>> tempTimedActions;
		unique_lock<mutex> uLock3(timedActionMutex);
		for (int i = 0; i < timedActions.size(); i++) {

			LOG(LogLevel::Debug) << "int AudioComponent::Update() : process clock : " << clock;
			LOG(LogLevel::Debug) << "int AudioComponent::Update() : frame time : " << clock->GetElapsedFrameTime();

			float processedTime = timedActions[i].first - clock->GetElapsedFrameTime();
			if (processedTime <= 0)
				timedActions[i].second();
			else {
				tempTimedActions.push_back(pair<float, function<int()>>(processedTime, timedActions[i].second));
			}
		}
		timedActions.clear();
		timedActions.assign(tempTimedActions.begin(), tempTimedActions.end());
	}
	

	return 0;
}

int AudioComponent::SetClock(FrameBasedClock * c)
{
	LOG(LogLevel::Debug) << "clock set [" << c << "]";

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
