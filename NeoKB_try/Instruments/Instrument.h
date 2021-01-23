#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../Framework/Allocation/Hierachal/Container.h"
#include "Audio/SoundBinding.h"
#include "Audio/SoundBindingSet.h"
#include <map>
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/KeyBindings/KeyBinding.h"
#include "../Framework/Audio/AudioManager.h"
#include "../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../Framework/Audio/Sample/SampleChannel.h"
#include "../Framework/Input/PassThroughInputManager.h"
#include "../Framework/Host/GameHost.h"
#include "Input/PitchBindingSet.h"


using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;
using namespace std;
using namespace Framework::Audio::Samples;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Audio;
using namespace Framework::Audio::Samples;
using namespace Framework::Input;
using namespace Framework::Host;
using namespace Instruments::Input;


namespace Framework {
namespace Host {

	//class GameHost;

}}


namespace Instruments {


	class Instrument : public Container {

		int load();

		int load(AudioManager* audioManager);

	public:

		Instrument();

		virtual PassThroughInputManager* CreateInputManager() = 0;

		virtual vector<KeyBinding*>* GetDefaultkeyBindings(int variant = 0) = 0;

		virtual vector<SoundBinding*>* GetDefaultSoundBindings(int variant = 0) = 0;

		virtual int SwitchSoundBindings(TSoundBindingSet<Pitch>* soundBindingSet) = 0;

		/// <summary>
		/// �o�ӬO�ΨӦb�t���L�{���A����U�����ഫ�����ХΪ��C���L�A�ΦAruleset�W�Ainstrument�����D�n����
		/// �b���U�^��ɡA�ثe�O������piano action������sound�A�p�G��pitch binding���ܡA�N�i�H���qpiano action������pitch�A�A��pitch�h������sound�A�|������b
		/// ���L�{�b����ۧ�A�i�H���g�npitch binding set�N�n�A�H�ᦳ�ݭn�A��C
		/// �t�~�bvirtual meteo piano�N�ܩ���A�]��virtual meteo piano�O������pitch�h�����n���A�y��get sample by pitch�Mget samples��ӥ\�୫�ƤF
		///	</summary>
		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0) = 0;

		//�g���F
		//virtual vector<PitchBinding*>* GetDefaultPitchBindings(int variant = 0) = 0;

		virtual int SetHost(GameHost* h);

		virtual int LoadAndMapSamples();

		virtual int LoadAndMapPitches();

	protected:

		GameHost* host = nullptr;

		AudioManager* audioManager = nullptr;

		vector<SoundBinding*> soundBindings;

		TSoundBindingSet<Pitch>* soundBindingSet = nullptr;

		PitchBindingSet* pitchBindingSet = nullptr;

		//�g���F
		//vector<PitchBinding*> pitchBindings;

		virtual int LoadOnComplete();

		virtual int loadAndMapSamples() = 0;

		/// <summary>
		/// �o�����ӬO�Φb��pitchBindingSet���]�w�n�A����n��pitch���ɭԴN������
		///	</summary>
		virtual int loadAndMapPitches() = 0;

		//�g���F
		//virtual int mapActionToPitch() = 0;

		string getSoundBindingFileName(int action);

		SoundBinding* getSoundBinding(int action);

	};

	template<typename T>
	class TInstrument : public Instrument, public KeyBindingHandler<T> {


	public:

		TInstrument() : RegisterType("TInstrument")
		{
			isInputable = true;
		}

	protected:

		map<T, SampleChannel*> samples;

		virtual map<T, SampleChannel*>* getSamples(int variant = 0) {
			return &samples;
		}

	};


}




#endif