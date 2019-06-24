#ifndef PAUSE_CONTAINER_H
#define PAUSE_CONTAINER_H



#include "../Scene.h"

using namespace Base::Scene;


namespace Base {
namespace Play {


	/// <summary>
	/// osu�����S���Ȱ���A�Ȱ��N�n�Y�p�����A�ҥH�S���H�|�h����pause container�A�ӬOpause container�ۤv�h�߰�host�O�_active�A�p�G
	/// �����Y�p�~�|pause
	/// 
	/// �����k���ӬO �Aplayfield load���ɭԡA��Event processor��algorithm add child�A���o��algorithm��pause container�����
	/// �M��bplayfield add event processor���ɭԡA�A��algorithm�\�J�o��processor�̡C
	///
	/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
	/// </summary>
	class PauseContainer: public Scene {

	public:

		/// <summary>
		/// pause container�������O�bplayer�̭��������A���Opause container�ۤv��
		/// </summary>
		int SetFramedClock(FramedClock* fClock);

		int SetAdjustableClock(AdjustableClock* aClock);


	protected:


	private:

		int load();

		/// <summary>
		/// �bupdate���ˬd���S���Q�Ȱ��A�����ܴN��ۤv��������
		/// </summary>
		int update();

		/// <summary>
		/// �@�ӻPparent�W�ߪ��ɲסA�U���������C��������A�C���ھڳo�Ӯ����B��
		/// </summary>
		FramedClock* framedClock;

		AdjustableClock* audioClock;

	};

}}


#endif