#ifndef SONG_PROGRESS_H
#define SONG_PROGRESS_H



#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../Framework/Timing/AdjustableClock.h"



using namespace Framework::Allocation::Hierachal;
using namespace Framework::Timing;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {


	/// <summary>
	/// ����p�G�n�Τ���ޱ��ɶ��A�n�A�g�@��song progress controller�AUI����A�A�Ψ��ӥh��input�A���H��L�ӱ���o��
	/// </summary>
	class SongProgress : public Container {

	public:

		SongProgress();

		int SetTimeSource(AdjustableClock* tSource);

		AdjustableClock* GetTimeSource();

		int SetIsAllowSeek(bool iAllowSeek);

		template<class _Type>
		int AddOnSeek(_Type* callableObject, function<int(double)> callback, string name = "HandleSeek") {
			onSeek.Add(callableObject, callback, name);
		}

	protected:

		bool isAllowSeek = true;

		AdjustableClock* timeSource = nullptr;

		ActionList<int, double> onSeek;

	};



}}}}


#endif