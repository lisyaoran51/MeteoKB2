#ifndef OCTAVE_AUTO_CONTROL_POINT_H
#define OCTAVE_AUTO_CONTROL_POINT_H


#include "AutoControlPoint.h"
#include "../../../../Framework/Input/KeyBindings/HasInputKey.h"



using namespace Framework::Input::KeyBindings;



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	/// <summary>
	/// �����K�ת��S�ġA���G�S���Ψ�
	/// �o�䪺life time�O�������K�ײ��L�h��h�֮ɶ��A���O�������o�ӭ��쪺�ɪ��h�[
	///	</summary>
	class OctaveAutoControlPoint : public AutoControlPoint, public HasInputKey {


	public:

		OctaveAutoControlPoint(InputKey iKey, MTO_FLOAT s, MTO_FLOAT l);

		OctaveAutoControlPoint(InputKey iKey, int iValue, MTO_FLOAT s, MTO_FLOAT l);

		virtual ControlPointType GetControlPointType();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

		virtual Event* Clone();

	};

}}}}



#endif
