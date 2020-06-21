#ifndef INSTRUMENT_BASS_SAMPLE_CHANNEL_H
#define INSTRUMENT_BASS_SAMPLE_CHANNEL_H


#include "MultiPlaybackSampleChannel.h"



namespace Framework {
namespace Audio {
namespace Samples {

	/// <summary>
	/// 電子樂器一個音我們分為前段和後段，前段正常播放以後，逐漸fadeout，並且讓後段fadein，後段則使用loop讓一小段音檔不停播放，節省記憶體
	/// 前段是head，前後段交替是overlapping，後段是tail
	/// </summary>
	enum class SampleChannelSetState {
		None,
		Stop,
		Head,
		Overlapping,
		Tail
	};

	/// <summary>
	/// tail也可以切成很多段，每段有不同的fadeout速度
	/// 我們這邊先設定最多切四段，不過實際上切到兩段效果就很好了，例如第一段tail維持8秒，音量降到一半，第二段維持12秒，降到0
	/// 總延音長度達到20秒效果就會步錯
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
		/// 目前的原始音量，會因為channel fadeout而改變大小，但是不會被音量調整功能影響
		/// </summary>
		float CurrentVolume = 1.0;

		double ElapsedTime = 0;

		double FadeOutTimeLeft = 0;
	};


	/// <summary>
	/// 模仿電子樂器將一段聲音切成好幾段，每段用不同方式處理，來降低記憶體。
	/// 我們切成AB段，A段是按下去前幾秒，是用原聲，B段是按下去的尾音，用不停的loop降低記憶體消耗
	/// 比較麻煩的是bass不支援AB段無縫接軌，所以A段和B 段中間會有一個斷掉的空白，我們得用fade out把AB段重疊在一起
	/// A段切換至B段的時間點是cut time，AB段重疊的時間長度是overlap length
	/// </summary>
	class InstrumentBassSampleChannel : public MultiPlaybackSampleChannel {

	public:

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType = OverrideType::MinimunVolume);

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, OverrideType oType, Sample* eSample, float cTime, float oLength);

		InstrumentBassSampleChannel(Sample* s, int pAmount, int tAmount, Sample* eSample, float cTime, float oLength);

		virtual InstrumentBassSampleChannel* Initialize(Sample* eSample, float cTime, float oLength);

		/// <summary>
		/// 增加一個tail fade out的區段，加在所有tail interval最後面
		/// tail interval意思是B段的音效會延遲一段時間，並且在該段時間慢慢Fade out，這段延遲期間稱為tail interval length
		/// 並且要指定這個tail interval最後要Fade out到多少音量，也就是tail Interval End Volume
		/// </summary>
		virtual InstrumentBassSampleChannel* AddTailInterval(float tailIntervalLength, float tailIntervalEndVolume);

		virtual int Update();

		virtual int Play(int trackNumber);

		virtual int Play(int trackNumber, double v);

		virtual int Stop(int trackNumber);

		//virtual int Stop(int channelId) = 0; 不會有這個狀況，要停就是所有都停，所以先不寫

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
		/// cut time是指在第幾秒由A段切換至B段
		/// </summary>
		double cutTime = 0;

		/// <summary>
		/// overlap length是指A段切換至B段中的重疊時間多久
		/// </summary>
		double overlapLength = 0;

		/// <summary>
		/// pair裡面第一個參數是tail interval length，這段區間的時間長度
		/// pair裡面第二個參數是tail interval end volume，這段區間最後音量要fadeout到多少
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