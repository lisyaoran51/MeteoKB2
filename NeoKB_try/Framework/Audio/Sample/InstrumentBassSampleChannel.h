#ifndef INSTRUMENT_BASS_SAMPLE_CHANNEL_H
#define INSTRUMENT_BASS_SAMPLE_CHANNEL_H


#include "MultiPlaybackSampleChannel.h"



namespace Framework {
namespace Audio {
namespace Samples {

	/// <summary>
	/// �q�l�־��@�ӭ��ڭ̤����e�q�M��q�A�e�q���`����H��A�v��fadeout�A�åB����qfadein�A��q�h�ϥ�loop���@�p�q���ɤ�������A�`�ٰO����
	/// �e�q�Ohead�A�e��q����Ooverlapping�A��q�Otail
	/// </summary>
	enum class SampleChannelSetState {
		None,
		Stop,
		Head,
		Overlapping,
		Tail
	};

	/// <summary>
	/// tail�]�i�H�����ܦh�q�A�C�q�����P��fadeout�t��
	/// �ڭ̳o����]�w�̦h���|�q�A���L��ڤW�����q�ĪG�N�ܦn�F�A�Ҧp�Ĥ@�qtail����8��A���q����@�b�A�ĤG�q����12��A����0
	/// �`�������׹F��20��ĪG�N�|�B��
	/// </summary>
	enum class SampleChannelTailState {
		None = -1,
		First,
		Second,
		Third,
		Last
	};

	class InstrumentSampleChannelSet {
	public:
		int StartChannelID = -1;
		int EndChannelId = -1;
		SampleChannelSetState State = SampleChannelSetState::None;
		SampleChannelTailState TailState = SampleChannelTailState::None;
		
		/// <summary>
		/// �ثe����l���q�A�|�]��channel fadeout�ӧ��ܤj�p�A���O���|�Q���q�վ�\��v�T
		/// </summary>
		float CurrentVolume = 1.0;

		double ElapsedTime = 0;

		double FadeOutTimeLeft = 0;
	};


	/// <summary>
	/// �ҥ�q�l�־��N�@�q�n�������n�X�q�A�C�q�Τ��P�覡�B�z�A�ӭ��C�O����C
	/// �ڭ̤���AB�q�AA�q�O���U�h�e�X��A�O�έ��n�AB�q�O���U�h�������A�Τ�����loop���C�O�������
	/// ����·Ъ��Obass���䴩AB�q�L�_���y�A�ҥHA�q�MB �q�����|���@���_�����ťաA�ڭ̱o��fade out��AB�q���|�b�@�_
	/// A�q������B�q���ɶ��I�Ocut time�AAB�q���|���ɶ����׬Ooverlap length
	/// </summary>
	class InstrumentBassSampleChannel : public MultiPlaybackSampleChannel {

	public:

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType = OverrideType::MinimunVolume);

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType, Sample* eSample, float cTime, float oLength);

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, Sample* eSample, float cTime, float oLength);

		virtual InstrumentBassSampleChannel* Initialize(Sample* eSample, float cTime, float oLength);

		/// <summary>
		/// �W�[�@��tail fade out���Ϭq�A�[�b�Ҧ�tail interval�̫᭱
		/// tail interval�N��OB�q�����ķ|����@�q�ɶ��A�åB�b�Ӭq�ɶ��C�CFade out�A�o�q��������٬�tail interval length
		/// �åB�n���w�o��tail interval�̫�nFade out��h�֭��q�A�]�N�Otail Interval End Volume
		/// </summary>
		virtual InstrumentBassSampleChannel* AddTailInterval(float tailIntervalLength, float tailIntervalEndVolume);

		virtual int Update();

		virtual int Play(int trackNumber);

		virtual int Play(int trackNumber, double v);

		virtual int Stop(int trackNumber);

		//virtual int Stop(int channelId) = 0; ���|���o�Ӫ��p�A�n���N�O�Ҧ������A�ҥH�����g

		virtual int FadeOut();

		virtual int FadeOut(int trackNumber);

		virtual int StopFadeOut();

		virtual int StopFadeOut(int trackNumber);

		virtual bool GetIsPlaying();

		virtual bool GetIsPlaying(int trackNumber);

		virtual bool GetIsLoaded();

		virtual int OnStateChange(int trackNumber);

	protected:

		/// <summary>
		/// cut time�O���b�ĴX���A�q������B�q
		/// </summary>
		double cutTime = 0;

		/// <summary>
		/// overlap length�O��A�q������B�q�������|�ɶ��h�[
		/// </summary>
		double overlapLength = 0;

		/// <summary>
		/// pair�̭��Ĥ@�ӰѼƬOtail interval length�A�o�q�϶����ɶ�����
		/// pair�̭��ĤG�ӰѼƬOtail interval end volume�A�o�q�϶��̫᭵�q�nfadeout��h��
		/// </summary>
		vector<pair<float, float>> tailInterval;

		Sample* endSample = nullptr;

		InstrumentSampleChannelSet** sampleChannelSets = nullptr;

		virtual int getChannelToPlay(int trackNumber);

		virtual InstrumentSampleChannelSet* getSetToPlay(int trackNumber);

		virtual InstrumentSampleChannelSet* getSetFromChannelId(int channelId);

		virtual int createSampleChannel();

		virtual int createEndSampleChannel();

		virtual int stopSampleChannelSet(InstrumentSampleChannelSet* channelSet);

	private:

		bool InstrumentBassSampleChannelInitialized = false;

		int construct(Sample* eSample, float cTime, float oLength);

		float getTailIntervalTimeLeft(InstrumentSampleChannelSet* channelSet);

		float getTailIntervalEndVolume(InstrumentSampleChannelSet* channelSet);

	};




}}}



#endif