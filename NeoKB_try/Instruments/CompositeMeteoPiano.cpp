#include "CompositeMeteoPiano.h"


using namespace Instruments;



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
	virtualMeteoPiano->SetSustainType(VirtualMeteoPianoSustainType::Pedal);

	registerLoad(bind(static_cast<int(CompositeMeteoPiano::*)(void)>(&CompositeMeteoPiano::load), this));
}

int CompositeMeteoPiano::SetHost(GameHost * h)
{
	return meteoPiano->SetHost(h);
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


	return 0;
}

int CompositeMeteoPiano::ChangePitchState(MeteoPianoPitchState s)
{
	return meteoPiano->ChangePitchState(s);
}

VirtualMeteoPiano * CompositeMeteoPiano::GetVirtualMeteoPiano()
{
	return virtualMeteoPiano;
}

int CompositeMeteoPiano::loadAndMapSamples()
{
	if (GetLoadState() == LoadState::Loaded) {
		meteoPiano->LoadAndMapSamples();
		virtualMeteoPiano->LoadAndMapSamples();
	}
	return 0;
}
