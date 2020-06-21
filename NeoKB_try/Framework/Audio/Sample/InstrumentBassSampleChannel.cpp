#include "InstrumentBassSampleChannel.h"



#include "BassSample.h"

using namespace Framework::Audio::Samples;
using namespace std;




InstrumentBassSampleChannel::InstrumentBassSampleChannel(Sample * s, int pAmount, int tAmount, OverrideType oType) : MultiPlaybackSampleChannel(s, pAmount, tAmount, oType)
{
}

InstrumentBassSampleChannel::InstrumentBassSampleChannel(Sample * s, int pAmount, int tAmount, OverrideType oType, Sample* eSample, float cTime, float oLength): MultiPlaybackSampleChannel(s, pAmount, tAmount, oType)
{
	construct(eSample, cTime, oLength);
}

InstrumentBassSampleChannel::InstrumentBassSampleChannel(Sample * s, int pAmount, int tAmount, Sample* eSample, float cTime, float oLength) : MultiPlaybackSampleChannel(s, pAmount, tAmount, OverrideType::MinimunVolume)
{
	construct(eSample, cTime, oLength);
}

InstrumentBassSampleChannel * InstrumentBassSampleChannel::Initialize(Sample * eSample, float cTime, float oLength)
{
	construct(eSample, cTime, oLength);
	return this;
}

InstrumentBassSampleChannel * InstrumentBassSampleChannel::AddTailInterval(float tailIntervalTime, float tailIntervalEndVolume)
{
	tailInterval.push_back(pair<float, float>(tailIntervalTime, tailIntervalEndVolume));

	return this;
}

int InstrumentBassSampleChannel::Update()
{
	AudioComponent::Update();

	if (!InstrumentBassSampleChannelInitialized)
		return 0;

	for (int i = 0; i < trackAmount; i++) {
		for (int j = 0; j < playbackAmount; j++) {

			switch (sampleChannelSets[i][j].State) {
			case SampleChannelSetState::Head:
			case SampleChannelSetState::Overlapping:
			case SampleChannelSetState::Tail:
				sampleChannelSets[i][j].ElapsedTime += clock->GetElapsedFrameTime();
				break;
			case SampleChannelSetState::None:
			case SampleChannelSetState::Stop:
				continue;
				break;
			}

			// �@�뼽�񪬪p
			if (!isTrackFadingOut[i]) {

				switch (sampleChannelSets[i][j].State) {
				case SampleChannelSetState::Head:
					if (sampleChannelSets[i][j].ElapsedTime > cutTime) {

						unique_lock<mutex> uLock(pendingActionMutex);
						pendingActions.Add(this, [=]() {
							// fade out A�q
							if (BASS_ChannelIsActive(sampleChannelSets[i][j].StartChannelID) == BASS_ACTIVE_PLAYING) {
								BASS_ChannelSlideAttribute(sampleChannelSets[i][j].StartChannelID, BASS_ATTRIB_VOL, 0, (DWORD)(overlapLength * 1000));
							}
							// fade in B�q
							if (!BASS_ChannelIsActive(sampleChannelSets[i][j].EndChannelId) == BASS_ACTIVE_PLAYING) {

								BASS_ChannelSetAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_PAN, 0);
								BASS_ChannelSetAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, 0);
								BASS_ChannelSetPosition(sampleChannelSets[i][j].EndChannelId, 0, BASS_POS_BYTE);

								BASS_ChannelPlay(sampleChannelSets[i][j].EndChannelId, true);

								BASS_ChannelSlideAttribute(sampleChannelSets[i][j].StartChannelID, BASS_ATTRIB_VOL, sampleChannelSets[i][j].CurrentVolume * volumeCalculated->GetValue(), (DWORD)(overlapLength * 1000));
							}
							sampleChannelSets[i][j].State = SampleChannelSetState::Overlapping;
							return 0;
						}, "Lambda_InstrumentBassSampleChannel::StartOverlap");
					}
					break;

				case SampleChannelSetState::Overlapping:
					// overlap�ഫ��tail
					if (sampleChannelSets[i][j].ElapsedTime > cutTime + overlapLength) {

						unique_lock<mutex> uLock(pendingActionMutex);
						pendingActions.Add(this, [=]() {

							sampleChannelSets[i][j].State = SampleChannelSetState::Tail;
							sampleChannelSets[i][j].TailState = SampleChannelTailState::First;

							float timeLeft = getTailIntervalTimeLeft(&sampleChannelSets[i][j]); // �o��tail interval�٭n�h�[slide��
							float endVolume = getTailIntervalEndVolume(&sampleChannelSets[i][j]); // �o��tail interval�̫�nslide��h�j���q

							if (BASS_ChannelIsActive(sampleChannelSets[i][j].EndChannelId) == BASS_ACTIVE_PLAYING) {
								BASS_ChannelSlideAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, endVolume * volumeCalculated->GetValue(), (DWORD)(timeLeft * 1000));
							}

							
							return 0;
						}, "Lambda_InstrumentBassSampleChannel::StartTail");
					}
					break;

				case SampleChannelSetState::Tail:
					// �ഫ�ܤU�@��tail state�άO��������
					if (getTailIntervalTimeLeft(&sampleChannelSets[i][j]) < 0) {

						if (tailInterval.size() > (int)sampleChannelSets[i][j].TailState + 1) {	// �N���٦��U�@��tail state�A�����٨S����

							unique_lock<mutex> uLock(pendingActionMutex);
							pendingActions.Add(this, [=]() {
								sampleChannelSets[i][j].TailState = SampleChannelTailState((int)sampleChannelSets[i][j].TailState + 1);	// �i�J�U�@��tail state

								float timeLeft = getTailIntervalTimeLeft(&sampleChannelSets[i][j]); // �o��tail interval�٭n�h�[slide��
								float endVolume = getTailIntervalEndVolume(&sampleChannelSets[i][j]); // �o��tail interval�̫�nslide��h�j���q

								if (BASS_ChannelIsActive(sampleChannelSets[i][j].EndChannelId) == BASS_ACTIVE_PLAYING) {
									BASS_ChannelSlideAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, endVolume * volumeCalculated->GetValue(), (DWORD)(timeLeft * 1000));
								}
								return 0;
							}, "Lambda_InstrumentBassSampleChannel::NextTail");
						}
						else {	// �N��w�g�O�̫�@��state�A�n�N�o��channel����

							unique_lock<mutex> uLock(pendingActionMutex);
							pendingActions.Add(this, [=]() {

								stopSampleChannelSet(&sampleChannelSets[i][j]);

								return 0;
							}, "Lambda_InstrumentBassSampleChannel::EndTail");
						}

					}

				}
			}
			// Fade out�ɪ����p
			else {
				switch (sampleChannelSets[i][j].State) {
				case SampleChannelSetState::Head:
				case SampleChannelSetState::Overlapping:
				case SampleChannelSetState::Tail:
					sampleChannelSets[i][j].FadeOutTimeLeft -= clock->GetElapsedFrameTime();
					break;
				}

				/*
				 * Fadeout����
				 */
				if (sampleChannelSets[i][j].FadeOutTimeLeft <= 0) {
					unique_lock<mutex> uLock(pendingActionMutex);
					pendingActions.Add(this, [=]() {

						stopSampleChannelSet(&sampleChannelSets[i][j]);

						return 0;
					}, "Lambda_InstrumentBassSampleChannel::FadeOutOver");
				}
				/*
				 * Fadeout������
				 */
				else {
					switch (sampleChannelSets[i][j].State) {
					case SampleChannelSetState::Head:
						if (sampleChannelSets[i][j].ElapsedTime > cutTime) {

							unique_lock<mutex> uLock(pendingActionMutex);
							pendingActions.Add(this, [=]() {

								/*
								 * �p�G�boverlap���ɭ�fadeout�N�����F�A�N����B�q�A����fadeout��A�q
								 * �p�Goverlap��fadeout�٨S�����A�N�٬O����B�q�A���LB�q���q�u�nslide��overlap�����ɪ����q�j�p�N�n
								 */
								if (overlapLength < sampleChannelSets[i][j].FadeOutTimeLeft) {
									// fade out A�q
									if (BASS_ChannelIsActive(sampleChannelSets[i][j].StartChannelID) == BASS_ACTIVE_PLAYING) {
										BASS_ChannelSlideAttribute(sampleChannelSets[i][j].StartChannelID, BASS_ATTRIB_VOL, 0, (DWORD)(overlapLength * 1000));
									}

									// fade in B�q
									float overlapEndVolume = sampleChannelSets[i][j].CurrentVolume - sampleChannelSets[i][j].CurrentVolume / sampleChannelSets[i][j].FadeOutTimeLeft * overlapLength;	// overlap�����ɪ����q = �ثe���q - overlap�����|fadeout�����q

									if (!BASS_ChannelIsActive(sampleChannelSets[i][j].EndChannelId) == BASS_ACTIVE_PLAYING) {

										BASS_ChannelSetAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_PAN, 0);
										BASS_ChannelSetAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, 0);
										BASS_ChannelSetPosition(sampleChannelSets[i][j].EndChannelId, 0, BASS_POS_BYTE);

										BASS_ChannelPlay(sampleChannelSets[i][j].EndChannelId, true);

										BASS_ChannelSlideAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, overlapEndVolume * volumeCalculated->GetValue(), (DWORD)(overlapLength * 1000));
									}
								}

								sampleChannelSets[i][j].State = SampleChannelSetState::Overlapping;
								return 0;
							}, "Lambda_InstrumentBassSampleChannel::StartOverlap");
						}
						break;

					case SampleChannelSetState::Overlapping:
						if (sampleChannelSets[i][j].ElapsedTime > cutTime + overlapLength) {

							unique_lock<mutex> uLock(pendingActionMutex);
							pendingActions.Add(this, [=]() {

								if (tailInterval.size() > 0) {

									sampleChannelSets[i][j].State = SampleChannelSetState::Tail;
									sampleChannelSets[i][j].TailState = SampleChannelTailState::First;

									if (BASS_ChannelIsActive(sampleChannelSets[i][j].EndChannelId) == BASS_ACTIVE_PLAYING) {
										BASS_ChannelSlideAttribute(sampleChannelSets[i][j].EndChannelId, BASS_ATTRIB_VOL, 0, (DWORD)(sampleChannelSets[i][j].FadeOutTimeLeft * 1000));
									}

								}
								return 0;
							}, "Lambda_InstrumentBassSampleChannel::StartTail");

						}
						break;

					case SampleChannelSetState::Tail:
						if (getTailIntervalTimeLeft(&sampleChannelSets[i][j]) <= 0) {
							if (tailInterval.size() > (int)sampleChannelSets[i][j].TailState + 1) {	// �N���٦��U�@��tail state�A�����٨S����

								sampleChannelSets[i][j].TailState = SampleChannelTailState((int)sampleChannelSets[i][j].TailState + 1);

							}
							else {
								unique_lock<mutex> uLock(pendingActionMutex);
								pendingActions.Add(this, [=]() {

									stopSampleChannelSet(&sampleChannelSets[i][j]);

									return 0;
								}, "Lambda_InstrumentBassSampleChannel::FadeOutOver");
							
							}
						}
						break;
					}
				}
			}
		}
	}
	return 0;
}

int InstrumentBassSampleChannel::Play(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::Play() : track number out of range.");

	if (!InstrumentBassSampleChannelInitialized)
		throw runtime_error("int InstrumentBassSampleChannel::Play() : channel not initialized.");

	return Play(trackNumber, 1.0);
}

int InstrumentBassSampleChannel::Play(int trackNumber, double v)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::Play() : track number out of range.");

	if (!InstrumentBassSampleChannelInitialized)
		throw runtime_error("int InstrumentBassSampleChannel::Play() : channel not initialized.");


	if (!GetIsLoaded())
		return -1;

	/* ���N�ثe���bfadeout�������Ȱ�fadeout�A�A����s���� */
	StopFadeOut(trackNumber);

	unique_lock<mutex> uLock(pendingActionMutex);


	int channelId = getChannelToPlay(trackNumber);
	InstrumentSampleChannelSet* sampleChannelSet = getSetFromChannelId(channelId);


	LOG(LogLevel::Depricated) << "InstrumentBassSampleChannel::Play() : get channel [" << channelId << "] to play.";

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Depricated) << "InstrumentBassSampleChannel::Play() : set channel with volume [" << v * volumeCalculated->GetValue() * 2.0 << "].";

		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_PAN, 0);
		BASS_ChannelSetAttribute(channelId, BASS_ATTRIB_VOL, v * volumeCalculated->GetValue());

		sampleChannelSet->CurrentVolume = v;
		sampleChannelSet->ElapsedTime = 0;

		return 0;
	}, "Lambda_InstrumentBassSampleChannel::SetChannel");

	pendingActions.Add(this, [=]() {

		LOG(LogLevel::Depricated) << "InstrumentBassSampleChannel::Play() : play channel.";

		BASS_ChannelPause(channelId);
		BASS_ChannelSetPosition(channelId, 0, BASS_POS_BYTE);

		BASS_ChannelPause(sampleChannelSet->EndChannelId);
		BASS_ChannelSetPosition(sampleChannelSet->EndChannelId, 0, BASS_POS_BYTE);

		BASS_ChannelPlay(channelId, true);
		sampleChannelSet->State = SampleChannelSetState::Head;
		sampleChannelSet->TailState = SampleChannelTailState::None;

		return 0;
	}, "Lambda_InstrumentBassSampleChannel::Play");

	return 0;
}

int InstrumentBassSampleChannel::Stop(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::Stop() : track number out of range.");

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {

			stopSampleChannelSet(&sampleChannelSets[i][trackNumber]);
		}
		
		return 0;
	}, "Lambda_InstrumentBassSampleChannel::Stop");

	return 0;
}

int InstrumentBassSampleChannel::FadeOut()
{
	for (int i = 0; i < trackAmount; i++)
		FadeOut(i);

	return 0;
}

int InstrumentBassSampleChannel::FadeOut(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::FadeOut() : track number out of range.");

	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		if (isTrackFadingOut[trackNumber])
			return 0;

		isTrackFadingOut[trackNumber] = true;

		for (int i = 0; i < playbackAmount; i++) {

			InstrumentSampleChannelSet* tempSet = &sampleChannelSets[trackNumber][i];

			switch (tempSet->State) {

			case SampleChannelSetState::Head:
				if (BASS_ChannelIsActive(tempSet->StartChannelID) == BASS_ACTIVE_PLAYING) {
					BASS_ChannelSlideAttribute(tempSet->StartChannelID, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * tempSet->CurrentVolume * 1000));
				}
				break;

			case SampleChannelSetState::Overlapping:

				double currentTimeLocal = BASS_ChannelBytes2Seconds(
					tempSet->StartChannelID,
					BASS_ChannelGetPosition(tempSet->StartChannelID, BASS_POS_BYTE));

				if (overlapLength - (currentTimeLocal - cutTime) > fadeOutTime) {
					/*
					 * overlap������fadeout�N�w�g�����F�A�N�n������A�qfade out���A�åB�P��fadeout�{��B�q�����q
					 */
					if (BASS_ChannelIsActive(tempSet->StartChannelID) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSlideAttribute(tempSet->StartChannelID, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * tempSet->CurrentVolume * 1000));
					}
					if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * tempSet->CurrentVolume * 1000));
					}
				}
				else if (overlapLength - (currentTimeLocal - cutTime) < fadeOutTime) {
					/*
					 * overlap������fadeout�٨S�����A�N��B�q�C�Cfadeout
					 */
					if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * tempSet->CurrentVolume * 1000));
					}
				}
				break;

			case SampleChannelSetState::Tail:
				if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
					BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, 0, (DWORD)(fadeOutTime * tempSet->CurrentVolume * 1000));
				}
				break;

			}

			tempSet->FadeOutTimeLeft = fadeOutTime * tempSet->CurrentVolume;

		}

		return 0;
	}, "Lambda_InstrumentBassSampleChannel::FadeOut");
}

int InstrumentBassSampleChannel::StopFadeOut()
{
	for (int i = 0; i < trackAmount; i++)
		StopFadeOut(i);
	return 0;
}

int InstrumentBassSampleChannel::StopFadeOut(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::StopFadeOut() : track number out of range.");


	unique_lock<mutex> uLock(pendingActionMutex);

	pendingActions.Add(this, [=]() {

		if (!isTrackFadingOut[trackNumber])
			return 0;

		isTrackFadingOut[trackNumber] = false;

		for (int i = 0; i < playbackAmount; i++) {

			InstrumentSampleChannelSet* tempSet = &sampleChannelSets[trackNumber][i];

			switch (tempSet->State) {

			case SampleChannelSetState::Head:

				if (BASS_ChannelIsActive(tempSet->StartChannelID) == BASS_ACTIVE_PLAYING) {
					BASS_ChannelSetAttribute(tempSet->StartChannelID, BASS_ATTRIB_VOL, tempSet->CurrentVolume * volumeCalculated->GetValue());
				}
				break;

			case SampleChannelSetState::Overlapping:

				double currentTimeLocal = BASS_ChannelBytes2Seconds(
					tempSet->StartChannelID,
					BASS_ChannelGetPosition(tempSet->StartChannelID, BASS_POS_BYTE));

				if (overlapLength - (currentTimeLocal - cutTime) < 0)
					continue;

				if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
					BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, tempSet->CurrentVolume * volumeCalculated->GetValue(), (DWORD)(overlapLength - (currentTimeLocal - cutTime) * 1000));
				}
				break;

			case SampleChannelSetState::Tail:

				float timeLeft = getTailIntervalTimeLeft(tempSet); // �o��tail interval�٭n�h�[slide��
				float endVolume = getTailIntervalEndVolume(tempSet); // �o��tail interval�̫�nslide��h�j���q



				if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
					BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, endVolume * tempSet->CurrentVolume * volumeCalculated->GetValue(), (DWORD)timeLeft);
				}
				break;


			}

			tempSet->FadeOutTimeLeft = 0;
		}

		return 0;
	}, "Lambda_InstrumentBassSampleChannel::StopFadeOut");

}

bool InstrumentBassSampleChannel::GetIsPlaying()
{
	for (int i = 0; i < trackAmount; i++)
		if (GetIsPlaying(i))
			return true;

	return false;
}

bool InstrumentBassSampleChannel::GetIsPlaying(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::GetIsPlaying() : track number out of range.");

	for (int i = 0; i < playbackAmount; i++) {
		if (sampleChannelSets[trackNumber][i].State != SampleChannelSetState::None &&
			sampleChannelSets[trackNumber][i].State != SampleChannelSetState::Stop) {
			return true;
		}
	}

	return false;
}

bool InstrumentBassSampleChannel::GetIsLoaded()
{
	return sample->GetIsLoaded() && endSample->GetIsLoaded();
}

int InstrumentBassSampleChannel::OnStateChange(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::OnStateChange() : track number out of range.");

	AdjustableAudioComponent::OnStateChange();

	unique_lock<mutex> uLock(pendingActionMutex);

	pendingActions.Add(this, [=]() {

		for (int i = 0; i < playbackAmount; i++) {

			InstrumentSampleChannelSet* tempSet = &sampleChannelSets[trackNumber][i];

			if (!isTrackFadingOut[trackNumber]) {

				switch (tempSet->State) {

				case SampleChannelSetState::Head:
					if (BASS_ChannelIsActive(tempSet->StartChannelID) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSetAttribute(channelIds[trackNumber][i], BASS_ATTRIB_VOL, tempSet->CurrentVolume * volumeCalculated->GetValue());
					}
					break;

				case SampleChannelSetState::Overlapping:

					double currentTimeLocal = BASS_ChannelBytes2Seconds(
						tempSet->StartChannelID,
						BASS_ChannelGetPosition(tempSet->StartChannelID, BASS_POS_BYTE));

					if (overlapLength - (currentTimeLocal - cutTime) < 0)
						continue;

					if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, tempSet->CurrentVolume * volumeCalculated->GetValue(), (DWORD)(overlapLength - (currentTimeLocal - cutTime) * 1000));
					}
					break;

				case SampleChannelSetState::Tail:

					float timeLeft = getTailIntervalTimeLeft(tempSet); // �o��tail interval�٭n�h�[slide��
					float endVolume = getTailIntervalEndVolume(tempSet); // �o��tail interval�̫�nslide��h�j���q

					if (BASS_ChannelIsActive(tempSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
						BASS_ChannelSlideAttribute(tempSet->EndChannelId, BASS_ATTRIB_VOL, endVolume * tempSet->CurrentVolume * volumeCalculated->GetValue(), (DWORD)timeLeft);
					}
					break;

				}
			}
			else {
				// TODO: �o���٨S�g
			}
		}


		return 0;
	}, "Lambda_InstrumentBassSampleChannel::OnStateChange");

	return 0;
}

int InstrumentBassSampleChannel::getChannelToPlay(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::getChannelToPlay() : track number out of range.");

	int channelToPlay = -1;
	for (int i = 0; i < playbackAmount; i++) {
		if (sampleChannelSets[trackNumber][i].State == SampleChannelSetState::None ||
			sampleChannelSets[trackNumber][i].State == SampleChannelSetState::Stop) {
			channelToPlay = sampleChannelSets[trackNumber][i].StartChannelID;
		}
	}
	
	if (channelToPlay == -1) {

		float minVol = 1;
		int minVolChannel = -1;

		for (int i = 0; i < playbackAmount; i++) {

			/* ���q�I���: ���q = ��l���q * EXP(-�ɶ�) */
			float tempVol = sampleChannelSets[trackNumber][i].CurrentVolume * exp(-sampleChannelSets[trackNumber][i].ElapsedTime);

			if (tempVol < minVol) {
				minVol = tempVol;
				minVolChannel = sampleChannelSets[trackNumber][i].StartChannelID;
			}
		}

		channelToPlay = minVolChannel;
	}

	return channelToPlay;
}

InstrumentSampleChannelSet * InstrumentBassSampleChannel::getSetToPlay(int trackNumber)
{
	if (trackNumber >= trackAmount)
		throw runtime_error("int InstrumentBassSampleChannel::getChannelToPlay() : track number out of range.");

	InstrumentSampleChannelSet* setToPlay = nullptr;
	for (int i = 0; i < playbackAmount; i++) {
		if (sampleChannelSets[trackNumber][i].State == SampleChannelSetState::None ||
			sampleChannelSets[trackNumber][i].State == SampleChannelSetState::Stop) {
			setToPlay = &(sampleChannelSets[trackNumber][i]);
		}
	}

	if (setToPlay == nullptr) {

		float minVol = 1;
		InstrumentSampleChannelSet* minVolSet = nullptr;

		for (int i = 0; i < playbackAmount; i++) {

			/* ���q�I���: ���q = ��l���q * EXP(-�ɶ�) */
			float tempVol = sampleChannelSets[trackNumber][i].CurrentVolume * exp(-sampleChannelSets[trackNumber][i].ElapsedTime);

			if (tempVol < minVol) {
				minVol = tempVol;
				minVolSet = &(sampleChannelSets[trackNumber][i]);
			}
		}

		setToPlay = minVolSet;
	}

	return setToPlay;
}

InstrumentSampleChannelSet * InstrumentBassSampleChannel::getSetFromChannelId(int channelId)
{
	for (int i = 0; i < trackAmount; i++) {
		for (int j = 0; j < playbackAmount; j++) {
			if (sampleChannelSets[i][j].StartChannelID == channelId ||
				sampleChannelSets[i][j].EndChannelId == channelId) {
				return &(sampleChannelSets[i][j]);
			}
		}
	}

	throw runtime_error("int InstrumentBassSampleChannel::getSetFromChannelId() : channel id not found in sets.");

	return nullptr;
}

int InstrumentBassSampleChannel::createSampleChannel()
{
	int channelId = dynamic_cast<BassSample*>(sample)->CreateChannel();

	LOG(LogLevel::Depricated) << "InstrumentBassSampleChannel::createSampleChannel() : get channel id [" << channelId << "].";

	return channelId;
}

int InstrumentBassSampleChannel::createEndSampleChannel()
{
	int channelId = dynamic_cast<BassSample*>(endSample)->CreateChannel();

	LOG(LogLevel::Depricated) << "InstrumentBassSampleChannel::createEndSampleChannel() : get channel id [" << channelId << "].";

	return channelId;
}

int InstrumentBassSampleChannel::stopSampleChannelSet(InstrumentSampleChannelSet * channelSet)
{
	if (BASS_ChannelIsActive(channelSet->StartChannelID) == BASS_ACTIVE_PLAYING) {
		BASS_ChannelPause(channelSet->StartChannelID);
	}
	BASS_ChannelSetPosition(channelSet->StartChannelID, 0, BASS_POS_BYTE);

	if (BASS_ChannelIsActive(channelSet->EndChannelId) == BASS_ACTIVE_PLAYING) {
		BASS_ChannelPause(channelSet->EndChannelId);
	}
	BASS_ChannelSetPosition(channelSet->EndChannelId, 0, BASS_POS_BYTE);

	channelSet->State = SampleChannelSetState::Stop;
	channelSet->TailState = SampleChannelTailState::None;
	channelSet->CurrentVolume = 0;
	channelSet->ElapsedTime = 0;
	channelSet->FadeOutTimeLeft = 0;
	return 0;
}

int InstrumentBassSampleChannel::construct(Sample* eSample, float cTime, float oLength)
{
	cutTime = cTime;
	overlapLength = oLength;
	endSample = eSample;

	sampleChannelSets = new InstrumentSampleChannelSet*[trackAmount];
	for (int i = 0; i < trackAmount; i++) {
		sampleChannelSets[i] = new InstrumentSampleChannelSet[playbackAmount];

		for (int j = 0; j < playbackAmount; j++) {
			sampleChannelSets[i][j].StartChannelID = channelIds[i][j];
			sampleChannelSets[i][j].EndChannelId = -1;
			sampleChannelSets[i][j].State = SampleChannelSetState::None;
		}
	}




	unique_lock<mutex> uLock(pendingActionMutex);
	pendingActions.Add(this, [=]() {

		for (int i = 0; i < trackAmount; i++) {

			for (int j = 0; j < playbackAmount; j++) {
				sampleChannelSets[i][j].EndChannelId = createEndSampleChannel();
				sampleChannelSets[i][j].State = SampleChannelSetState::Stop;

			}

		}

		return 0;
	}, "Lambda_InstrumentBassSampleChannel::CreateEndSampleChannels");

	//unique_lock<mutex> uLock(permanentActionMutex);
	//permanentActions.Add(this, bind(&InstrumentBassSampleChannel::checkCutTime, this), "InstrumentBassSampleChannel::checkCutTime");

	InstrumentBassSampleChannelInitialized = true;

	return 0;
}

float InstrumentBassSampleChannel::getTailIntervalTimeLeft(InstrumentSampleChannelSet * channelSet)
{
	if (channelSet->TailState == SampleChannelTailState::None)
		return -1;

	float totalTimeLeft = cutTime + overlapLength;
	for (int i = 0; i < (int)channelSet->TailState + 1; i++) {
		totalTimeLeft += tailInterval[i].first;
	}

	return totalTimeLeft - channelSet->ElapsedTime;
}

float InstrumentBassSampleChannel::getTailIntervalEndVolume(InstrumentSampleChannelSet * channelSet)
{
	if (channelSet->TailState == SampleChannelTailState::None)
		return 0;

	return tailInterval[(int)channelSet->TailState].second;
}
