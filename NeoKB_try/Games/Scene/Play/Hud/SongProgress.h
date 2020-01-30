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
	/// 之後如果要用手機操控時間，要再寫一個song progress controller再UI那邊，再用那個去接input，抓到以後過來控制這個
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