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
		/// 這個是用來在演奏過程中，把按下的鍵轉換成樂譜用的。部過適用再ruleset上，instrument不知道要怎麼用
		/// 在按下琴鍵時，目前是直接用piano action對應到sound，如果用pitch binding的話，就可以先從piano action對應到pitch，再用pitch去對應到sound，會比較乾淨
		/// 不過現在不急著改，可以先寫好pitch binding set就好，以後有需要再改。
		/// 另外在virtual meteo piano就很明顯，因為virtual meteo piano是直接用pitch去播放聲音，造成get sample by pitch和get samples兩個功能重複了
		///	</summary>
		virtual PitchBindingSet* GetDefaultPitchBindingSet(int variant = 0) = 0;

		//寫錯了
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

		//寫錯了
		//vector<PitchBinding*> pitchBindings;

		virtual int LoadOnComplete();

		virtual int loadAndMapSamples() = 0;

		/// <summary>
		/// 這個應該是用在把pitchBindingSet給設定好，之後要抓pitch的時候就直接用
		///	</summary>
		virtual int loadAndMapPitches() = 0;

		//寫錯了
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