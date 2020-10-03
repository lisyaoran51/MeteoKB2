#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "../Allocation/Hierachal/Container.h"
#include "SpeedAdjusters/SpeedAdjuster.h"
#include "DecoupledInterpolatingFramedClock.h"
#include "../Input/KeyBindings/KeyBindingHandler.h"
#include "../Input/Commands/CommandHandler.h"


using namespace Framework::Timing::SpeedAdjusters;
using namespace Framework::Timing;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Input::KeyBindings;
using namespace Framework::Input::Commands;



namespace Framework {
namespace Timing {

	/// <summary>
	/// �\�bplayer�U���A�Ψӱ���C���t�שM�Ȱ��B���p�`
	///	</summary>
	/// <summary>
	/// osu�����S���Ȱ���A�Ȱ��N�n�Y�p�����A�ҥH�S���H�|�h����pause container�A�ӬOpause container�ۤv�h�߰�host�O�_active�A�p�G
	/// �����Y�p�~�|pause
	/// 
	/// �����k���ӬO �Aplayfield load���ɭԡA��Event processor��algorithm add child�A���o��algorithm��pause container�����
	/// �M��bplayfield add event processor���ɭԡA�A��algorithm�\�J�o��processor�̡C
	///
	/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
	/// ���o�Ӥ��Oscene�A���Ӳ���timing�h
	/// </summary>
	class TimeController : public Container {

		int load();

	public:

		TimeController();
		~TimeController();

		/// <summary>
		/// �o�Ӯ������������N�Oaudio clock�A���L�L�i�H����audio�d��H�~���ɶ�
		/// </summary>
		int SetControllableClock(AdjustableClock* aClock);

		AdjustableClock* GetControllableClock();

		int SetSpeedAdjuster(SpeedAdjuster* sAdjuster);

		int JumpTo(double seekTime);

		int JumpToWithSpeedAdjust(double seekTime);

		int Pause();

		/// <summary>
		/// �o��n�P�_�p�G�O�badjust���e�N�w�gpause�F�A��resume�N�|���ѡC�|����pause�����~�|����
		/// </summary>
		int Resume();

		int SetRate(double rate);

		double GetRate();

		bool GetIsPaused();

		int SetIsAllowSeek(bool iAllowSeek);

		bool GetIsAllowSeek();

		template<class _Type>
		int AddOnRetry(_Type* callableObject, function<int()> callback, string name = "HandleRetryRequest") {
			onRetryRequested.Add(callableObject, callback, name);
			return 0;
		}

		template<class _Type>
		int AddOnQuit(_Type* callableObject, function<int()> callback, string name = "HandleQuitRequest") {
			onQuitRequested.Add(callableObject, callback, name);
			return 0;
		}

		/* �Ȯɤ��g�o�q�A�H���T��n���g�A�^�ӧ�
		int ImportWorkingSm(WorkingSm* workingSm);
		*/

	protected:


		/// <summary>
		/// �bupdate���ˬd���S���Q�Ȱ��A�����ܴN��ۤv��������
		/// </summary>
		virtual int update();

		/// <summary>
		/// �b�վ�ɶ����ɭԡA�ΨӰ���t�׮ĪG���F��
		/// </summary>
		SpeedAdjuster* speedAdjuster = nullptr;

		/// <summary>
		/// �i�H����q���������_����clock�A�N�O�q������clock�A�~���]�@���B�~������
		/// </summary>
		AdjustableClock* controllableClock = nullptr;

		/// <summary>
		/// �@�ӻPparent�W�ߪ��ɲסA�U���������C��������A�C���ھڳo�Ӯ����B��
		/// </summary>
		//FramedClock* gameClock;

		bool isAllowSeek = true;

		double rate = 1.0;

		double targetSeekTime;

		double defaultFreezeTime = 1.0;
		double defaultAdjustTime = 5.0;

		bool isPaused = false;
		bool isAdjustAfterPause = false;

		/// <summary>
		/// ��Ȱ������A�n�~��C���ɡA���ӭ˼Ʈɶ��A�o��isWaitingFreeze�N�|�Otrue
		/// </summary>
		bool isWaitingFreeze = false;

		ActionList<int> onRetryRequested;
		ActionList<int> onQuitRequested;


		/* �Ȯɤ��g�o�q�A�H���T��n���g�A�^�ӧ�

		WorkingSm* workingSm;

		int getTempSection();

		int getTempPart();

		vector<float> sectionStartTime;

		vector<float> partStartTime;

		*/
	private:

	};

	/// <summary>
	/// ���o�Ӥ��Oscene�A���Ӳ���timing�h
	/// </summary>
	template<typename T, typename TCommand>
	class TTimeController : public TimeController, public KeyBindingHandler<T>, public CommandHandler<TCommand> {

		int load() {

			// �ݭn��input key��binding�@�U�A�~���D���ӬOpause
			// ���ӬO����bind�Ainput manager��bind�n�F�A�ڭ̪����δN�n


			return 0;
		}

	public:

		TTimeController(): RegisterType("TTimeController") {

			registerLoad(bind(static_cast<int(TTimeController<T, TCommand>::*)(void)>(&TTimeController<T>::load), this));

		}

		//virtual map<T, InputKey>* GetDefaultkeyBindings() = 0;

		virtual int OnKeyDown(pair<T, int> action) {
			return 0;
		}

		virtual int OnKeyUp(T action) {
			return 0;
		}

		/// <summary>
		/// �o�q�g���F�A���n��
		/// </summary>
		virtual int OnButtonDown(T action) {
			typename map<T, InputKey>::iterator it;
			it = keyBindings.find(action);

			LOG(LogLevel::Debug) << "TTimeController::OnButtonDown(): check bindings " << (keyBindings.find(action) == keyBindings.end() ? "not found" : "found");
			if (keyBindings[action] == InputKey::Pause) {
				LOG(LogLevel::Debug) << "TTimeController::OnButtonDown() : get pause button input and pause.";
				if (speedAdjuster->GetIsAdjustingTime())
					return -1;

				if (!GetIsPaused()) {
					Pause();
					SetAllChildsIsMaskedForTrigger();
				}
				else if(!isWaitingFreeze){
					speedAdjuster->SetFreezeTime(defaultFreezeTime);
					isWaitingFreeze = true;
				}
			}
			return 0;
		}

		virtual int OnButtonUp(T action) {
			return 0;
		}

		/// <summary>
		/// �o�q�g���F�A���n��
		/// </summary>
		virtual int OnKnobTurn(pair<T, int> action) {
			if (keyBindings[action.first] == InputKey::SpeedKnob) {
				SetRate(GetRate() + action.second);


			}
			if (keyBindings[action.first] == InputKey::SectionKnob) {

				if (isWaitingFreeze)
					return 0;

				//JumpTo(sectionStartTime[getTempSection() + action.second]);
				if (isPaused)
					isAdjustAfterPause = true;
				speedAdjuster->SetSeekTime(action.second * defaultAdjustTime);


			}
			return 0;
		}

		virtual int OnSlide(pair<T, int> action) {
			return 0;
		}

		virtual int OnCommand(TCommand* command) {
			return 0;
		}

	protected:


		map<T, InputKey> keyBindings;

		/// <summary>
		/// �bload�������ɭԡA�ɰO�n���檺�u�@
		/// </summary>
		virtual int LoadOnComplete() {

			//reloadMappings();

			return 0;
		}

		/// <summary>
		/// ��input key�M�s����J���X�@�U
		/// </summary>
		//virtual int reloadMappings() = 0;


	};

	





}}







#endif