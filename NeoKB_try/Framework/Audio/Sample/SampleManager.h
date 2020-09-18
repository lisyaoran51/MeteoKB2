#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H



#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../IO/Stores/CompositeResourceStore.h"
#include <map>
#include "BassSampleChannelGenerator.h"
#include "../../IO/Api/ApiAccess.h"



using namespace std;
using namespace Framework::IO::Stores;
using namespace Framework::IO::Api;




namespace Framework {
namespace Audio {
namespace Samples {

	class SampleManager : public AudioCollectionManager<SampleChannel> {


		ApiAccess* apiAccess = nullptr;

		CompositeResourceStore<char*>* resourceStore;

		map<string, SampleChannel*> sampleChannelCache;

	public:

		SampleManager(CompositeResourceStore<char*>* rStore);

		SampleChannel* GetSampleChannel(string name);

		SampleChannel* GetSampleChannel(SoundBinding* soundBinding);

		/// <summary>
		/// 部會再用了，可以刪掉
		/// </summary>
		SampleChannel* GetMultiPlaybackSampleChannel(string name);

		/// <summary>
		/// 部會再用了，可以刪掉
		/// </summary>
		int SetPlaybackAmount(int pAmount);

		int RemoveSampleChannel(string name);

		int RemoveSampleChannel(SoundBinding* soundBinding);

		int ClearSampleChannels();

		/* -------------------- 載入新音色 -------------------- */

		int SetupApiAccess(ApiAccess* aAccess);

		int Import(vector<string>* paths);

		/// <summary>
		/// 從系統預設的路徑去抓檔案
		/// </summary>
		int ImportFromStable();

		vector<SoundBindingSet*>* GetSoundBindingSets();

		/* -------------------- 更換音色 -------------------- */



	protected:

		/// <summary>
		/// 部會再用了，可以刪掉
		/// </summary>
		int playbackAmount = 10;

		vector<SoundBindingSet*>* soundBindingSets;

		SampleChannelGenerator* sampleChannelGenerator = nullptr;

		/// <summary>
		/// 會把這些路競裡面的檔案全都讀出來，並且用sm set做成一組一組樂譜，然後佔存到某個地方(資料庫或是?)
		/// 但是目前沒有做到這個部分，只把樂譜資料先丟進去一個list裡，詳建osu的作法
		/// </summary>
		vector<SoundBindingSet*>* import(FileReader& fileReader);

		/// <summary>
		/// 這個的功能指示把sm裡面的投檔資料讀出來，裡面的event還不需要，裡面的event要等到遊戲開始時再讀
		/// </summary>
		SoundBindingSet* importToStorage(FileReader& fileReader);

	};


}}}





#endif