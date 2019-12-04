#include "DecoupledInterpolatingFramedClock.h"

#include "../../Util/MtoType.h"
#include "../../Util/Log.h"
#include <iomanip>


using namespace Framework::Timing;
using namespace Util;
using namespace std;


DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock(): InterpolatingFramedClock()
{
	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::DecoupledInterpolatingFramedClock() : rate = [" << GetRate() << "].";
	decoupledClock = new FramedClock(decoupledStopwatchClock = new StopwatchClock());
}

double DecoupledInterpolatingFramedClock::GetCurrentTime()
{
	LOG(LogLevel::Depricated) << "DecoupledInterpolatingFramedClock::GetCurrentTime() : get decoupled clock? [" << getIsUseDecoupledClock() << "].";
	// �Τ���������? �����������ɶ� : ����l�������ɶ�(�i��|����)
	return getIsUseDecoupledClock() ? decoupledClock->GetCurrentTime() : InterpolatingFramedClock::GetCurrentTime();
}

int DecoupledInterpolatingFramedClock::SetRate(double r)
{
	decoupledClock->SetRate(r);
	InterpolatingFramedClock::SetRate(r);
	
	return 0;
}

int DecoupledInterpolatingFramedClock::SetIsRunning(bool value)
{
	decoupledClock->SetIsRunning(value);
	if (framedSource != nullptr)
	if (getAdjustableSource()->Seek(GetCurrentTime()) == RETURN_AVAILABLE) {
		InterpolatingFramedClock::SetIsRunning(value);
	}

	return 0;
}

bool DecoupledInterpolatingFramedClock::GetIsRunning()
{
	return getIsUseDecoupledClock() ? decoupledClock->GetIsRunning() : InterpolatingFramedClock::GetIsRunning();
}

int DecoupledInterpolatingFramedClock::ChangeSource(Clock * s)
{
	if (s == nullptr)
		return 0;

	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ChangeSource : changing.";

	if (dynamic_cast<AdjustableClock*>(s))
		dynamic_cast<AdjustableClock*>(s)->Seek(GetCurrentTime());

	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ChangeSource : get framed source.";
	source = s;
	framedSource = dynamic_cast<FramedClock*>(s) != nullptr ? dynamic_cast<FrameBasedClock*>(s) : new FramedClock(s);
	return 0;
}

double DecoupledInterpolatingFramedClock::GetElapsedFrameTime()
{
	return getIsUseDecoupledClock() ? decoupledClock->GetElapsedFrameTime() : InterpolatingFramedClock::GetElapsedFrameTime();
}

int DecoupledInterpolatingFramedClock::ProcessFrame()
{
	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ProcessFrame() : 1 before process interpolating clock, " << decoupledClock->GetIsRunning();
	InterpolatingFramedClock::ProcessFrame();
	decoupledStopwatchClock->SetRate(InterpolatingFramedClock::GetRate());

	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ProcessFrame() : 2 after process interpolating clock, " << decoupledClock->GetIsRunning();
	LOG(LogLevel::Finest) << "DecoupledInterpolatingFramedClock::ProcessFrame() : process decoupled clock";
	decoupledClock->ProcessFrame();
	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ProcessFrame() : 3 after process dcoupled clock, " << decoupledClock->GetIsRunning();


	bool sourceRunning = InterpolatingFramedClock::GetIsRunning();
	LOG(LogLevel::Finest) << "DecoupledInterpolatingFramedClock::ProcessFrame() : determine if decoupled clock start. coupled = ["<< isCoupled << "], source running = [" << (sourceRunning ? 1 : 0) << "]" << ", rate = " << InterpolatingFramedClock::GetRate();
	// ***�������������@����۲{�b���ɶ�***
	// �u�n��������b�]�A���������N�@�w�n��ۭ�������ɶ��C���p�G������S�b�]���ܡA �N�n�ݦ��S��couple�A
	// ��couple�����p�A���������N�������U�ӫO���������@�˪��ɶ��C
	// �S��couple�����p�A���������N�\�������A�~��]�U�h
	if (isCoupled || sourceRunning) { // ���j�b�@�_�A�Ϊ̬O��������b�]�A���@�w�n���
		if (sourceRunning)
			decoupledStopwatchClock->Start();
		else {
			decoupledStopwatchClock->Stop();
		}
		decoupledStopwatchClock->Seek(GetCurrentTime());
		
	}
	else {
		LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::ProcessFrame() : when the source is not running, the decoupled clock running is [" << decoupledClock->GetIsRunning() << "] by [" << this << "].";
		LOG(LogLevel::Depricated) << "DecoupledInterpolatingFramedClock::ProcessFrame() : current time is [" << fixed << setprecision(5) << GetCurrentTime() << "].";
		if (decoupledClock->GetIsRunning()) {
			//if we're running but our source isn't, we should try a seek to see if it's capable to switch to it for the current value.
			// �o��O���}�C�����֪��a��
			LOG(LogLevel::Info) << "DecoupledInterpolatingFramedClock::ProcessFrame() : music start";
			Start();
		}
	}



	// �٨S�g
	return 0;
}

int DecoupledInterpolatingFramedClock::Reset()
{
	isCoupled = true;

	if (getAdjustableSource() != nullptr)
		getAdjustableSource()->Reset();
	decoupledStopwatchClock->Reset();
	return 0;
}

int DecoupledInterpolatingFramedClock::Start()
{
	if(getAdjustableSource() != nullptr)
	if(!getAdjustableSource()->GetIsRunning()){
		LOG(LogLevel::Info) << "DecoupledInterpolatingFramedClock::Start() : check if coupled [" << isCoupled << "] and seek time [" << GetCurrentTime() << "].";
		if (isCoupled || getAdjustableSource()->Seek(GetCurrentTime())) {
			//only start the source clock if our time values match.
			//this handles the case where we seeked to an unsupported value and the source clock is out of sync.
			LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::Start() : start audio clock";
			getAdjustableSource()->Start();
		}
	}
	decoupledStopwatchClock->Start();
	return 0;
}

int DecoupledInterpolatingFramedClock::Stop()
{

	if (getAdjustableSource() != nullptr)
		getAdjustableSource()->Stop();
	decoupledStopwatchClock->Stop();
	LOG(LogLevel::Debug) << "DecoupledInterpolatingFramedClock::Stop() : after stop, the decoupled clock running is [" << decoupledClock->GetIsRunning() << "] by [" << this << "].";
	return 0;
}

bool DecoupledInterpolatingFramedClock::Seek(double position)
{
	bool success = false; // ����n�^�Ӭݬݪ�l���O���Ofalse
	if (getAdjustableSource() != nullptr)
		success = getAdjustableSource()->Seek(position) == true;

	if (isCoupled)
	{
		decoupledStopwatchClock->Seek(getAdjustableSource() != nullptr ? getAdjustableSource()->GetCurrentTime() : position);
		return success;
	}

	if (!success)
		//if we failed to seek then stop the source and use decoupled mode.
		if (getAdjustableSource() != nullptr)
			getAdjustableSource()->Stop();

	return decoupledStopwatchClock->Seek(position);
}

int DecoupledInterpolatingFramedClock::ResetSpeedAdjustments()
{
	InterpolatingFramedClock::SetRate(1.0);
	return 0;
}

int DecoupledInterpolatingFramedClock::SetIsCoupled(bool value)
{
	isCoupled = value;
	return 0;
}

bool DecoupledInterpolatingFramedClock::GetIsCoupled()
{
	return isCoupled;
}

AdjustableClock * DecoupledInterpolatingFramedClock::getAdjustableSource()
{
	if (source == nullptr)
		return nullptr;
	return dynamic_cast<AdjustableClock*>(source);
}

bool DecoupledInterpolatingFramedClock::getIsUseDecoupledClock()
{
	if (framedSource == nullptr)
		return !isCoupled;
	else
		return !isCoupled && !framedSource->GetIsRunning();
}
