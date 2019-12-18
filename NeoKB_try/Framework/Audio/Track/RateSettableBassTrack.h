#ifndef RATE_SETTABLE_BASS_TRACK_H
#define RATE_SETTABLE_BASS_TRACK_H


#include "BassTrack.h"


namespace Framework {
namespace Audio {
namespace Tracks {


	/*
	 * ����<=>�W�v����
	 * ���� + p => �W�v * 1.059 ^ p    ��ln
	 * ���� + ln(p)/ln(1.059) => �W�v * p
	 * BASS_ChannelSetAttribute(stream2, BASS_ATTRIB_TEMPO_PITCH, -log(��������)/log(1.0594630943592953098f));
	 */

	/// <summary>
	/// ���t�׭n�ϥΨ�bass_fx��arm�����A�åB�n�`�N���t�׻P�������������⤽���G
	/// �t�� * p => ���� - ln(p)/ln(1.0594630943592953098)
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