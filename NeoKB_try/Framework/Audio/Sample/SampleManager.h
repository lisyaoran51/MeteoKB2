#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H



#include "../AudioCollectionManager.h"
#include "SampleChannel.h"
#include "../../IO/Stores/CompositeResourceStore.h"
#include <map>
#include "BassSampleChannelGenerator.h"
#include "../../../Instruments/Audio/SoundBindingSet.h"
#include "../../IO/Storage.h"
#include "../../IO/FileReader.h"



using namespace std;
using namespace Framework::IO::Stores;
using namespace Instruments::Audio;
using namespace Framework::IO;



namespace Framework {
namespace IO{
namespace Api{

	class ApiAccess;

}}}
using namespace Framework::IO::Api;


namespace Framework {
namespace Audio {
namespace Samples {

	class SampleManager : public AudioCollectionManager<SampleChannel> , public MtoObject{


		ApiAccess* apiAccess = nullptr;

		CompositeResourceStore<char*>* resourceStore;

		map<string, SampleChannel*> sampleChannelCache;

	public:

		SampleManager(CompositeResourceStore<char*>* rStore);

		bool HasSampleChannel(string name);

		bool HasSampleChannel(SoundBinding* soundBinding);

		SampleChannel* GetSimpleSampleChannel(string name);

		SampleChannel* GetSampleChannel(string name);

		SampleChannel* GetSampleChannel(SoundBinding* soundBinding);

		/// <summary>
		/// ���|�A�ΤF�A�i�H�R��
		/// </summary>
		SampleChannel* GetMultiPlaybackSampleChannel(string name);

		/// <summary>
		/// ���|�A�ΤF�A�i�H�R��
		/// </summary>
		int SetPlaybackAmount(int pAmount);

		int RemoveSampleChannel(string name);

		int RemoveSampleChannel(SoundBinding* soundBinding);

		int ClearSampleChannels();

		/* -------------------- ���J�s���� -------------------- */

		/// <summary>
		/// �o�����ӭn�����H�令��sound package request�Ӱ�
		/// </summary>
		int SetupApiAccess(ApiAccess* aAccess);

		int Import(vector<string>* paths);

		/// <summary>
		/// �q�t�ιw�]�����|�h���ɮ�
		/// </summary>
		int ImportFromStable();

		vector<SoundBindingSet*>* GetSoundBindingSets();

		/// <summary>
		/// �q�t�ιw�]�����|���Ҧ��x�s���e
		/// </summary>
		function<Storage*(void)> GetStableStorage;

		/* -------------------- �󴫭��� -------------------- */

	protected:

		/// <summary>
		/// ���|�A�ΤF�A�i�H�R��
		/// </summary>
		int playbackAmount = 10;

		vector<SoundBindingSet*>* soundBindingSets;

		SampleChannelGenerator* sampleChannelGenerator = nullptr;

		/// <summary>
		/// �|��o�Ǹ��v�̭����ɮץ���Ū�X�ӡA�åB��sm set�����@�դ@�ռ��СA�M����s��Y�Ӧa��(��Ʈw�άO?)
		/// ���O�ثe�S������o�ӳ����A�u����и�ƥ���i�h�@��list�̡A�ԫ�osu���@�k
		/// </summary>
		vector<SoundBindingSet*>* import(FileReader& fileReader);

		/// <summary>
		/// �o�Ӫ��\����ܧ�sm�̭������ɸ��Ū�X�ӡA�̭���event�٤��ݭn�A�̭���event�n����C���}�l�ɦAŪ
		/// </summary>
		SoundBindingSet* importToStorage(FileReader& fileReader);

	};


}}}





#endif