#ifndef HAND_TYPE_H
#define HAND_TYPE_H





namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	enum class HandType {

		/* ��@���� */
		None,
		LeftEasy,				// 1 ����²�� �p�G�S������k��A�N�@�ߥ���A�������ת��ܴN�@��²��
		LeftOther,				// 2 ����²��H�~����
		RightEasy,				// 3 �k��²��
		RightOther,				// 4 �k��²��H�~����
		Foot,					// 5 ��O
		OctaveShift,			// 6 ����
		Hidden,					// 7 �������k��B���ת����ÿO����
		All,					// 8 ���󳣥i�H

		/* �ƦX������ */
		LeftOnly,					// 9 �¥���
		LeftOnlyFoot,				// 10 �¥��� ��O
		LeftOnlyOctaveShift,		// 11 �¥��� ����
		HiddenLeftOnly,				// 12 �¥��� ���ÿO���� 

		RightOnly,					// 13 �¥k��
		RightOnlyFoot,				// 14 �¥k�� ��O 
		RightOnlyOctaveShift,		// 15 �¥k�� ����
		HiddenRightOnly,			// 16 �¥k�� ���ÿO���� 

		EasyOnly,					// 17 ��²��
		EasyOnlyFoot,				// 18 ��²�� ��O 
		EasyOnlyOctaveShift,		// 19 ��²�� ����
		HiddenEasyOnly,				// 20 ��²�� ���ÿO���� 

		HardOnly,					// 21 �§x��
		HardOnlyFoot,				// 22 �§x�� ��O 
		HardOnlyOctaveShift,		// 23 �§x�� ����
		HiddenHardOnly,				// 24 �§x�� ���ÿO���� 

		LeftEasyOnly,				// 25 �¥���²��
		LeftEasyOnlyFoot,			// 26 �¥���²�� ��O 
		LeftEasyOnlyOctaveShift,	// 27 �¥���²�� ����
		HiddenLeftEasyOnly,			// 28 �¥���²�� ���ÿO���� 

		LeftHardOnly,				// 29 �¥���x��
		LeftHardOnlyFoot,			// 30 �¥���x�� ��O
		LeftHardOnlyOctaveShift,	// 31 �¥���x�� ����
		HiddenLeftHardOnly,			// 32 �¥���x�� ���ÿO���� 

		RightEasyOnly,				// 33 �¥k��²��
		RightEasyOnlyFoot,			// 34 �¥k��²�� ��O
		RightEasyOnlyOctaveShift,	// 35 �¥k��²�� ����
		HiddenRightEasyOnly,		// 36 �¥k��²�� ���ÿO���� 

		RightHardOnly,				// 37 �¥k��x��
		RightHardOnlyFoot,			// 38 �¥k��x�� ��O
		RightHardOnlyOctaveShift,	// 39 �¥k��x�� ����
		HiddenRightHardOnly,		// 40 �¥k��x�� ���ÿO���� 
		//Auto,	// �o�ӬO���O�ª��n�R��? 6�q�`�O�����K��
	};



}}}}


#endif