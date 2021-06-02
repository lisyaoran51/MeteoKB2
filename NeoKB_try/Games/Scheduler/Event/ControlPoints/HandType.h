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
		LeftOnly = 11,				// 11 �¥���
		LeftOnlyFoot,				// 12 �¥��� ��O
		LeftOnlyOctaveShift,		// 13 �¥��� ����
		LeftOnlyHidden,				// 14 �¥��� ���ÿO���� 

		RightOnly,					// 15 �¥k��
		RightOnlyFoot,				// 16 �¥k�� ��O 
		RightOnlyOctaveShift,		// 17 �¥k�� ����
		RightOnlyHidden,			// 18 �¥k�� ���ÿO���� 

		EasyOnly,					// 19 ��²��
		EasyOnlyFoot,				// 20 ��²�� ��O 
		EasyOnlyOctaveShift,		// 21 ��²�� ����
		EasyOnlyHidden,				// 22 ��²�� ���ÿO���� 

		HardOnly,					// 23 �§x��
		HardOnlyFoot,				// 24 �§x�� ��O 
		HardOnlyOctaveShift,		// 25 �§x�� ����
		HardOnlyHidden,				// 26 �§x�� ���ÿO���� 

		LeftEasyOnly,				// 27 �¥���²��
		LeftEasyOnlyFoot,			// 28 �¥���²�� ��O 
		LeftEasyOnlyOctaveShift,	// 29 �¥���²�� ����
		LeftEasyOnlyHidden,			// 30 �¥���²�� ���ÿO���� 

		LeftHardOnly,				// 31 �¥���x��
		LeftHardOnlyFoot,			// 32 �¥���x�� ��O
		LeftHardOnlyOctaveShift,	// 33 �¥���x�� ����
		LeftHardOnlyHidden,			// 34 �¥���x�� ���ÿO���� 

		RightEasyOnly,				// 35 �¥k��²��
		RightEasyOnlyFoot,			// 36 �¥k��²�� ��O
		RightEasyOnlyOctaveShift,	// 37 �¥k��²�� ����
		RightEasyOnlyHidden,		// 38 �¥k��²�� ���ÿO���� 

		RightHardOnly,				// 39 �¥k��x��
		RightHardOnlyFoot,			// 40 �¥k��x�� ��O
		RightHardOnlyOctaveShift,	// 41 �¥k��x�� ����
		RightHardOnlyHidden,		// 42 �¥k��x�� ���ÿO���� 
		//Auto,	// �o�ӬO���O�ª��n�R��? 6�q�`�O�����K��
	};



}}}}


#endif