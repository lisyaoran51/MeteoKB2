#ifndef RATE_SETTABLE_BASS_TRACK_H
#define RATE_SETTABLE_BASS_TRACK_H


#include "BassTrack.h"


namespace Framework {
namespace Audio {
namespace Tracks {


	/*
	 * 音高<=>頻率公式
	 * 音高 + p => 頻率 * 1.059 ^ p    取ln
	 * 音高 + ln(p)/ln(1.059) => 頻率 * p
	 * BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_PITCH, -log(乘的倍數)/log(1.0594630943592953098f));
	 */

	/// <summary>
	/// 更改速度要使用到bass_fx的arm版本，並且要注意更改速度與音高之間的換算公式：
	/// 速度 * p => 音高 - ln(p)/ln(1.0594630943592953098)
	/// </summary>
	class RateSettableBassTrack : public BassTrack {

		const double frequencyToPitch = 1.0594630943592953098;

		const double logFrequencyToPitch = 0.0577622650466621534448563579645;

		float pitch = 0.f;

		HSTREAM sourceStream;

	public:

		RateSettableBassTrack(char* fileName);

		virtual int SetRate(double r);

	};





}}}








#endif