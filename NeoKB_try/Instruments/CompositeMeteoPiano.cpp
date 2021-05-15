#include "CompositeMeteoPiano.h"

#include "../Framework/Audio/Sample/BassSampleChannelGenerator.h"

using namespace Instruments;
using namespace Framework::Audio::Samples;



int CompositeMeteoPiano::load()
{
	AddChild(meteoPiano);

	AddChild(virtualMeteoPiano);

	return 0;
}

CompositeMeteoPiano::CompositeMeteoPiano(vector<string>& args) : MeteoPiano(args), RegisterType("CompositeMeteoPiano")
{
	meteoPiano = new MeteoPiano(args);
	virtualMeteoPiano = new VirtualMeteoPiano();
	virtualMeteoPiano->SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType::Pedal);

	registerLoad(bind(static_cast<int(CompositeMeteoPiano::*)(void)>(&CompositeMeteoPiano::load), this));
}

int CompositeMeteoPiano::SetHost(Host * h)
{
	meteoPiano->SetHost(h);

	instrumentInputHandler = new InstrumentInputHandler(this);
	instrumentInputHandler->Initialize(h);

	return 0;
}

PitchBindingSet * CompositeMeteoPiano::GetDefaultPitchBindingSet(int variant)
{
	return meteoPiano->GetDefaultPitchBindingSet(variant);
}

int CompositeMeteoPiano::Sleep()
{
	meteoPiano->Sleep();
	virtualMeteoPiano->Sleep();
	return 0;
}

int CompositeMeteoPiano::WakeUp()
{
	meteoPiano->WakeUp();
	virtualMeteoPiano->WakeUp();
	return 0;
}

PassThroughInputManager * CompositeMeteoPiano::CreateInputManager()
{
	return meteoPiano->CreateInputManager();
}

vector<SoundBinding*>* CompositeMeteoPiano::GetDefaultSoundBindings(int variant)
{
	return meteoPiano->GetDefaultSoundBindings(variant);
}

vector<KeyBinding*>* CompositeMeteoPiano::GetDefaultkeyBindings(int variant)
{
	return meteoPiano->GetDefaultkeyBindings(variant);
}

int CompositeMeteoPiano::ChangeSustainType(SustainType sType)
{
	return meteoPiano->ChangeSustainType(sType);
}

SustainType CompositeMeteoPiano::GetSustainType()
{
	return meteoPiano->GetSustainType();
}

int CompositeMeteoPiano::ControlSustainPedal(bool down)
{
	return meteoPiano->ControlSustainPedal(down);
}

int CompositeMeteoPiano::SetGameControllingPitchState(bool value)
{
	return meteoPiano->SetGameControllingPitchState(value);
}

int CompositeMeteoPiano::MoveOctave(PianoPitchMovement s)
{
	return meteoPiano->MoveOctave(s);
}

int CompositeMeteoPiano::SwitchSoundBindings(TSoundBindingSet<Pitch>* sBindingSet)
{
	LOG(LogLevel::Debug) << "CompositeMeteoPiano::SwitchSoundBindings() : switch piano sound to [" << sBindingSet->fileName << "].";

	meteoPiano->SwitchSoundBindings(sBindingSet);
	virtualMeteoPiano->SwitchSoundBindings(sBindingSet);

	BassSampleChannelGenerator::ClearOldSamples();

	return 0;
}

int CompositeMeteoPiano::OnDirectKeyDown(pair<PianoAction, int> action)
{
	return GetMeteoPiano()->OnDirectKeyDown(action);
}

int CompositeMeteoPiano::ChangePitchState(MeteoPianoPitchState s)
{
	return meteoPiano->ChangePitchState(s);
}

VirtualMeteoPiano * CompositeMeteoPiano::GetVirtualMeteoPiano()
{
	return virtualMeteoPiano;
}

MeteoPiano * CompositeMeteoPiano::GetMeteoPiano()
{
	return meteoPiano;
}

int CompositeMeteoPiano::loadAndMapSamples()
{
	// load的時候就會add child，就會執行了，為什麼這邊要讓他們在執行一次？
	//if (GetLoadState() == LoadState::Loaded) {
	//	meteoPiano->LoadAndMapSamples();
	//	virtualMeteoPiano->LoadAndMapSamples();
	//}
	return 0;
}

int CompositeMeteoPiano::loadAndMapPitches()
{
	return 0;
}

int CompositeMeteoPiano::OnKeyDown(pair<PianoAction, int> action)
{
	return GetMeteoPiano()->OnKeyDown(action);
}

int CompositeMeteoPiano::OnKeyUp(PianoAction action)
{
	return GetMeteoPiano()->OnKeyUp(action);
}

int CompositeMeteoPiano::OnButtonDown(PianoAction action)
{
	return GetMeteoPiano()->OnButtonDown(action);
}

int CompositeMeteoPiano::OnButtonUp(PianoAction action)
{
	return GetMeteoPiano()->OnButtonUp(action);
}

int CompositeMeteoPiano::OnKnobTurn(pair<PianoAction, int> action)
{
	return GetMeteoPiano()->OnKnobTurn(action);
}

int CompositeMeteoPiano::OnSlide(pair<PianoAction, int> action)
{
	return GetMeteoPiano()->OnSlide(action);
}
