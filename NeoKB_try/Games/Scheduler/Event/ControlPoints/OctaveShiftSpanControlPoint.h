#ifndef OCTAVE_SHIFT_SPAN_CONTROL_POINT_H
#define OCTAVE_SHIFT_SPAN_CONTROL_POINT_H


#include "MarkControlPoint.h"



namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{


	/// <summary>
	/// �b���ӭ���A�o�䤣���\�B�bNone������A�@�w�n�Olowered��raised�~�����n�إߦ��@��event
	///	</summary>
	enum class OctaveRange {
		None,		
		Lowered,
		Raised
	};


	/// <summary>
	/// �аO�վ�K�׭��쪺�_�l�I�P�����h�[�A�o�䪺life time�O���b�o�ӭ�������F�h�[
	///	</summary>
	class OctaveShiftSpanControlPoint : public MarkControlPoint {

	public:

		OctaveShiftSpanControlPoint(OctaveRange oRange, MTO_FLOAT s, MTO_FLOAT l);

		int SetOctaveRange(OctaveRange oRange);
		OctaveRange GetOctaveRange();

		// �@�w�n�C����override!!
		virtual string GetTypeName();

	protected:

		OctaveRange octaveRange = OctaveRange::None;

	};





}}}}






#endif