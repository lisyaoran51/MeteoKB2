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
	virtualMeteoPiano->SetVirtualMeteoPianoSustainType(VirtualMeteoPianoSustainType::Pedal);

	registerLoad(bind(static_cast<int(CompositeMeteoPiano::*)(void)>(&CompositeMeteoPiano::load), this));
}

int CompositeMeteoPiano::SetHost(GameHost * h)
{
	meteoPiano->SetHost(h);

	instrumentInputHandler = new InstrumentInputHandler(GetMeteoPiano());
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
